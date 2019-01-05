#include "drb_graphics.h"

#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define ARRAY_COUNT(arr) (sizeof(arr)/sizeof(arr[0]))

// ================================================================

// todo: this should be accessible from the header.
static const char* model_uni_name = "model";
static const char* projection_uni_name = "projection";
static const char* view_uni_name = "view";

static const char* text_position_uni_name = "position";

static const char* tex0_sampler_name = "tex0";

static float gfx_screen_size[2];

gfx_malloc_fptr gfx_malloc;
gfx_free_fptr gfx_free;
gfx_realloc_fptr gfx_realloc;

static void default_log(const char* txt, ...);
gfx_log_fptr gfx_text_log = default_log;

/*
      7--6
    3--2 |
    | 4|-5
    0--1
*/
float box_vertices[] = { 
    -0.5f, -0.5f, 0.5f,
     0.5f, -0.5f, 0.5f,
     0.5f,  0.5f, 0.5f,
    -0.5f,  0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f
};

uint32_t box_indices[] = {
    0, 1, 2,
    2, 3, 0,
    1, 5, 6,
    6, 2, 1,
    4, 0, 3,
    3, 7, 4,
    5, 4, 7,
    7, 6, 5,
    3, 2, 6,
    6, 7, 3,
    4, 5, 1,
    1, 0, 4
};

// ================================================================

static void default_log(const char* txt, ...)
{
    va_list argp;
    va_start(argp, txt);
    vprintf(txt, argp);
    va_end(argp);
    fflush(stdout);
}

// ----------------------------------------------------------------

static const char* texture_unit_sampler_name(uint32_t tex_i)
{
    switch(tex_i)
    {
        case 0:
        {
            return tex0_sampler_name;
        } break;
    }
    return 0;
}

// ----------------------------------------------------------------

static enum gfx_status create_gpu_mesh(struct gfx_mesh* mesh,
                                        const struct gfx_mesh_def* def)
{
    void* tmp_buf = 0;

    tmp_buf = gfx_malloc(def->nverts * gpu_max_vert_bytes);
    if (!tmp_buf)
        goto error;

    gpu_vtx_flags_t flags = gpu_pack_verts(tmp_buf, def->positions, 
        def->normals, def->texcoords, def->nverts);

    if(gpu_vertex_buffer_create(&mesh->vertex_buffer, tmp_buf, flags, def->indices, def->nverts, def->nindices) != GPU_OK) goto error;

    gfx_free(tmp_buf);

    return GFX_OK;

error:
    gfx_text_log("ERROR: Couldn't create GPU meshes.\n");
    gfx_free(tmp_buf);
    return GFX_FAILURE;
}

// ----------------------------------------------------------------

static enum gfx_status create_gpu_textures(struct gfx_mesh* mesh,
                                           const struct gfx_texture_def* tex_defs,
                                           uint32_t ntexdefs)
{
    int32_t tex_i = 0;
    for(; tex_i < (int32_t) ntexdefs; tex_i++)
    {
        if(gpu_texture_create_RGBA(&mesh->textures[tex_i], tex_defs[tex_i].data, tex_defs[tex_i].width, tex_defs[tex_i].height) != GPU_OK)
            goto error;
    }

    mesh->ntextures = ntexdefs;

    return GFX_OK;

error:
    gfx_text_log("ERROR: Failed to create gpu textures.");

    for(--tex_i; tex_i > 0; --tex_i)
    {
        gpu_texture_destroy(&mesh->textures[tex_i]);
    }

    return GFX_FAILURE;
}

// ----------------------------------------------------------------

static void add_point(float** ptr, float x, float y, float z)
{
    float* p = *ptr;
    p[0] = x;
    p[1] = y;
    p[2] = z;
    *ptr += 3;
}

// ----------------------------------------------------------------

static void add_triangle(uint32_t** ptr, uint32_t v0, uint32_t v1, uint32_t v2)
{
    uint32_t* p = *ptr;
    p[0] = v0;
    p[1] = v1;
    p[2] = v2;
    *ptr += 3;
}

// ================================================================

void gfx_set_log(gfx_log_fptr l) { gfx_text_log = l; }

// ----------------------------------------------------------------

enum gfx_status gfx_init()
{
    if (gpu_init() != GPU_OK)
        goto error;

    int screen_params[4];
    gpu_get_screen_params(screen_params);
    gfx_screen_size[0] = (float)screen_params[2];
    gfx_screen_size[1] = (float)screen_params[3];

    return GFX_OK;
    
error:
    gfx_text_log("ERROR: Failed to init graphics.\n");
    return GFX_FAILURE;
}

// ----------------------------------------------------------------

void gfx_set_mem(gfx_malloc_fptr m, gfx_free_fptr f, gfx_realloc_fptr r)
{
    gfx_malloc = m;
    gfx_free = f;
    gfx_realloc = r;
}

// ----------------------------------------------------------------

enum gfx_status gfx_activate_program(struct gfx_program* program)
{
    if(gpu_activate_program(program->program) != GPU_OK)
    {
        gfx_text_log("ERROR: Cannot activate program.\n");
        return GFX_FAILURE;
    }

    return GFX_OK;
}

// ----------------------------------------------------------------

void gfx_update_window(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

// ----------------------------------------------------------------

enum gfx_status gfx_update_view(const m4* view, struct gfx_program* program)
{
    gpu_uniform view_uni;
    if(gpu_get_uniform(program->program, view_uni_name, &view_uni) != GPU_OK)
        goto error;
    if(gpu_set_uniform_m4(view_uni, (float*)view) != GPU_OK)
        goto error;

    return GFX_OK;

error:
    gfx_text_log("ERROR: Failed to update view uniform.\n");
    return GFX_FAILURE;
}

// ----------------------------------------------------------------

enum gfx_status gfx_update_projection(const m4* projection, struct gfx_program* program)
{
    gpu_uniform projection_uni;
    if(gpu_get_uniform(program->program, projection_uni_name, &projection_uni) != GPU_OK)
        goto error;
    if(gpu_set_uniform_m4(projection_uni, (float*)projection) != GPU_OK)
        goto error;

    return GFX_OK;
    
error:
    gfx_text_log("ERROR: Failed to update projection uniform.\n");
    return GFX_FAILURE;
}

// ----------------------------------------------------------------

void gfx_box_create(struct gfx_mesh_def* out_box)
{
    *out_box = (struct gfx_mesh_def){};
    out_box->positions = box_vertices;
    out_box->indices = box_indices;
    out_box->nverts = ARRAY_COUNT(box_vertices) / 3;
    out_box->nindices = ARRAY_COUNT(box_indices);
}

void gfx_box_destroy(struct gfx_mesh_def* def)
{
}

void gfx_circle_create(uint32_t npoints, struct gfx_mesh_def* out_circle)
{
    *out_circle = (struct gfx_mesh_def){};

    const double PI = 3.14159265358979323846;
    const uint32_t NTHETA_INCS = npoints;
    const uint32_t NVERTS = NTHETA_INCS + 1;
    const uint32_t NTRIS = NVERTS;
    const double THETA_INC = (2.0 * PI) / NTHETA_INCS;

    float* verts = (float*)gfx_malloc((NVERTS * 3) * sizeof(float));
    float* curr_vert = verts;

    uint32_t* tris = (uint32_t*)gfx_malloc((NTRIS * 3) * sizeof(uint32_t));
    uint32_t* curr_tri = tris;

    double theta = 0.0;

    add_point(&curr_vert, 0.0f, 0.0f, 0.0f);
    for(uint32_t vert_i = 0; vert_i < NTHETA_INCS; ++vert_i)
    {
        add_point(&curr_vert, cos(theta), sin(theta), 0.0f);
        theta += THETA_INC;
    }

    for(uint32_t tri_i = 1; tri_i < NTRIS - 1; ++tri_i)
    {
        add_triangle(&curr_tri, 0, tri_i, tri_i + 1);
    }
    add_triangle(&curr_tri, 0, NTRIS - 1, 1);

    out_circle->positions = verts;
    out_circle->nverts = NVERTS;
    out_circle->indices = tris;
    out_circle->nindices = NTRIS * 3;
}

// ----------------------------------------------------------------

void gfx_circle_destroy(struct gfx_mesh_def* def)
{
    gfx_free(def->positions);
    gfx_free(def->indices);
}

// ----------------------------------------------------------------

void gfx_cylinder_create(uint32_t npoints, struct gfx_mesh_def* out_cylinder)
{

}

// ----------------------------------------------------------------

void gfx_cylinder_destroy(struct gfx_mesh_def* def)
{
    gfx_free(def->positions);
    gfx_free(def->indices);
}

// ----------------------------------------------------------------

enum gfx_status gfx_mesh_create(struct gfx_mesh* mesh, const struct gfx_mesh_def* def, const struct gfx_texture_def* tex_defs, uint32_t ntexdefs)
{
    *mesh = (struct gfx_mesh){};

    { // Create gpu_meshes
        if (create_gpu_mesh(mesh, def) != GFX_OK)
            goto error;
    }

    { // Create gpu textures
        mesh->textures = (struct gpu_texture*)gfx_malloc(sizeof(struct gpu_texture) * ntexdefs);
        if (!mesh->textures)
            goto error;

        if (create_gpu_textures(mesh, tex_defs, ntexdefs) != GFX_OK)
            goto error;
    }

    return GFX_OK;
error:
    gfx_text_log("ERROR: Failed to create graphics mesh.\n");

    gfx_mesh_destroy(mesh);

    return GFX_FAILURE;
}

// ----------------------------------------------------------------

void gfx_mesh_destroy(struct gfx_mesh* mesh)
{
    gpu_vertex_buffer_destroy(&mesh->vertex_buffer);

    for(uint32_t tex_i = 0; tex_i < mesh->ntextures; ++tex_i)
    {
        gpu_texture_destroy(&mesh->textures[tex_i]);
    }
    gfx_free(mesh->textures);

    *mesh = (struct gfx_mesh){};
}

// ----------------------------------------------------------------

enum gfx_status gfx_mesh_draw(struct gfx_mesh* mesh, const struct gfx_program* active_program, const m4* transforms, uint32_t ntransforms)
{
    for(uint32_t tex_i = 0; tex_i < mesh->ntextures; ++tex_i)
    {
        if (gpu_texture_bind(&mesh->textures[tex_i], &active_program->program, 1, tex_i, texture_unit_sampler_name(tex_i)) != GPU_OK)
            goto error;
    }

    gpu_uniform model;
    if(gpu_get_uniform(active_program->program, model_uni_name, &model) != GPU_OK)
       goto error;

    for(uint32_t trans_i = 0; trans_i < ntransforms; ++trans_i)
    {
        if(gpu_set_uniform_m4(model, (float*)&transforms[trans_i]) != GPU_OK)
        {
            gfx_text_log("ERROR: Cannot set model transform.\n");
            goto error;
        }

        gpu_vertex_buffer_draw(&mesh->vertex_buffer);
    }

    return GFX_OK;

error:
    gfx_text_log("ERROR: Failed to draw mesh.\n");
    return GFX_FAILURE;
}

// ----------------------------------------------------------------

enum gfx_status gfx_font_create(struct gfx_font* fnt, const struct gfx_font_def* def, bool_t transfer_glyphs)
{
    *fnt = (struct gfx_font){};
    if(gpu_texture_create_R(&fnt->texture, def->bitmap, def->bmp_width, def->bmp_height) != GPU_OK)
        goto error;

    fnt->nglyphs = def->nglyphs;
    fnt->bmp_height = def->bmp_height;
    fnt->bmp_width = def->bmp_width;
    fnt->line_spacing = def->line_spacing;
    if(transfer_glyphs)
    {
        fnt->glyphs = def->glyphs;
        fnt->owns_glyphs = 0;
    }
    else
    {
        size_t gbytes = sizeof(struct gfx_glyph) * fnt->nglyphs;
        fnt->glyphs = gfx_malloc(gbytes);
        if(fnt->glyphs == 0) 
            goto error;
        memcpy(fnt->glyphs, def->glyphs, gbytes);
        fnt->owns_glyphs = 1;
    }
    
    return GFX_OK;

error:
    gfx_font_destroy(fnt);
    gfx_text_log("ERROR: Failed to create graphics font.\n");
    return GFX_FAILURE;
}

// ----------------------------------------------------------------

void gfx_font_destroy(struct gfx_font* fnt)
{
    gpu_texture_destroy(&fnt->texture);
    if(fnt->owns_glyphs)
    {
        gfx_free(fnt->glyphs);
    }
    *fnt = (struct gfx_font){};
}

// ----------------------------------------------------------------

enum gfx_status gfx_text_create(struct gfx_text* txt, const char* text_ansi, struct gfx_font* font)
{
    *txt = (struct gfx_text){};

    const uint32_t max_supported_characters = 1024;

    float* positions = 0;
    float* texcoords = 0;
    uint32_t* indices = 0;
    void* packed_verts = 0;

    txt->text_ansi = text_ansi;
    txt->font = font;

    { // Layout the text
        const char* c = txt->text_ansi;

        float tex_width = (float) font->bmp_width;
        float tex_height = (float) font->bmp_height;

        uint32_t text_len = 0;
        {
            uint32_t count = 0;
            while(*(c + count++) != 0) {
                if(count > max_supported_characters)
                {
                    gfx_text_log("ERROR[gfx_text_create]: Maximum number of supported characters exceeded.\n");
                    goto error;
                }
            }
            text_len = count;
        }

        // 4 vertices times 3 floats each for each character
        positions = gfx_malloc(sizeof(float) * 12 * text_len );
        // 4 vertices times 3 floats each for each character
        texcoords = gfx_malloc(sizeof(float) * 12 * text_len );
        // 6 indices for each character
        indices = gfx_malloc(sizeof(uint32_t) * 6 * text_len );

        packed_verts = gfx_malloc(text_len * 4 * gpu_max_vert_bytes);

        if( positions == 0 || texcoords == 0 || indices == 0 || 
            packed_verts == 0)
        {
            gfx_text_log("ERROR: Out of memory.\n");
            goto error;
        }

        float* curr_pos = positions;
        float* curr_tex = texcoords;
        uint32_t* curr_idx = indices;
        uint32_t idx = 0;

        float start_x = 0.0f;
        float x = start_x;
        float y = -font->line_spacing / (gfx_screen_size[1] * 0.5f);;
        while(*c != '\0')
        {
            if( *c == '\n' )
            {
                y -= font->line_spacing / (gfx_screen_size[1] * 0.5f);
                x = start_x;
                c++;
                continue;
            }

            // Handle characters that don't have glyphs
            const struct gfx_glyph* glyph = 0;
            for(uint32_t glyph_i = 0; glyph_i < font->nglyphs; glyph_i++)
            {
                struct gfx_glyph* curr_glyph = &font->glyphs[glyph_i];
                if( curr_glyph->charcode == (uint64_t)*c )
                {
                    glyph = curr_glyph;
                    break;
                }
            }

            if(glyph == 0)
            {
                gfx_text_log("ERROR: Missing glyph for character \'%c\'\n", *c);
                goto error;
            }

            float tex_x = (float) glyph->x / tex_width;
            float tex_y = (float) glyph->y / tex_height;

            float width = (float) glyph->width / (gfx_screen_size[0] * 0.5f);
            float height = (float) glyph->height / (gfx_screen_size[1] * 0.5f);

            float width_tex = (float) glyph->width / tex_width;
            float height_tex = (float) glyph->height / tex_height;
            float advance = glyph->advance_x / (gfx_screen_size[0] * 0.5f);
            float bearing_x = glyph->bearing_x / (gfx_screen_size[0] * 0.5f);
            float bearing_y = glyph->bearing_y / (gfx_screen_size[1] * 0.5f);
            float pos_x = x + bearing_x;
            float pos_y = y + (bearing_y - height);

            x += advance;

            // don't create a quad for space character
            if( *c == ' ' )
            {
                c++;
                continue;
            }

            // bottom left
            *(curr_pos++) = pos_x;
            *(curr_pos++) = pos_y;
            *(curr_pos++) = 0.0f;
            *(curr_tex++) = tex_x;
            *(curr_tex++) = tex_y;
            *(curr_tex++) = 0.0f;            

            // bottom right
            *(curr_pos++) = pos_x + width;
            *(curr_pos++) = pos_y;
            *(curr_pos++) = 0.0f;
            *(curr_tex++) = tex_x + width_tex;
            *(curr_tex++) = tex_y;
            *(curr_tex++) = 0.0f;                        

            // top right
            *(curr_pos++) = pos_x + width;
            *(curr_pos++) = pos_y + height;
            *(curr_pos++) = 0.0f;
            *(curr_tex++) = tex_x + width_tex;
            *(curr_tex++) = tex_y + height_tex;
            *(curr_tex++) = 0.0f;         

            // top left
            *(curr_pos++) = pos_x;
            *(curr_pos++) = pos_y + height;
            *(curr_pos++) = 0.0f;
            *(curr_tex++) = tex_x;
            *(curr_tex++) = tex_y + height_tex;
            *(curr_tex++) = 0.0f;         

            *(curr_idx++) = idx;
            *(curr_idx++) = idx + 1;
            *(curr_idx++) = idx + 2;
            *(curr_idx++) = idx + 2;
            *(curr_idx++) = idx + 3;
            *(curr_idx++) = idx;

            idx += 4;
            
            c++;
        }

        ptrdiff_t nverts = (curr_pos - positions)/3;
        ptrdiff_t nindices = curr_idx - indices;
        gpu_vtx_flags_t flags = gpu_pack_verts(packed_verts, positions, 0, texcoords, nverts );

        gpu_vertex_buffer_create(&txt->quads, packed_verts, flags, indices, nverts, nindices );
    }

    gfx_free(packed_verts);
    gfx_free(positions);
    gfx_free(texcoords);
    gfx_free(indices);

    return GFX_OK;

error:
    gfx_free(packed_verts);
    gfx_free(positions);
    gfx_free(texcoords);
    gfx_free(indices);
    gpu_vertex_buffer_destroy(&txt->quads);

    *txt = (struct gfx_text){};

    gfx_text_log("ERROR: Failed to create graphics text: \"%s\".\n", text_ansi);
    return GFX_FAILURE;
}

// ----------------------------------------------------------------

void gfx_text_destroy(struct gfx_text* txt)
{
    gpu_vertex_buffer_destroy(&txt->quads);
    *txt = (struct gfx_text){};
}

// ----------------------------------------------------------------

enum gfx_status gfx_text_draw(struct gfx_text* txt, const struct gfx_program* active_program, float screen_x, float screen_y)
{
    gpu_uniform position_uni;
    float position[] = { screen_x, screen_y };

    struct gfx_font* font = txt->font;
   
    if (gpu_get_uniform(active_program->program, text_position_uni_name, &position_uni) != GPU_OK)
        goto error;

    { // Draw text
        if (gpu_texture_bind(&font->texture, &active_program->program, 1, 0, texture_unit_sampler_name(0)) != GPU_OK)
            goto error;

        if (gpu_set_uniform_2f(position_uni, position) != GPU_OK)
            goto error;

        gpu_vertex_buffer_draw(&txt->quads);
    }

    return GFX_OK;

error:
    gfx_text_log("ERROR: Cannot draw text.\n");
    return GFX_FAILURE;
}

// ----------------------------------------------------------------

void gfx_clear_screen(v3 color)
{
    gpu_clear_screen(color.x, color.y, color.z);
}