// todo:
// - update window size when it changes
// - default shader
// - high-level shader compilation functions
// - 

#pragma once

#include "drb_gpu.h"
#include "drb_math.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t bool_t;

typedef void (*gfx_log_fptr)(const char* txt, ...);
void gfx_set_log(gfx_log_fptr l);

extern gfx_log_fptr gfx_text_log;

typedef void* (*gfx_malloc_fptr)(size_t nbytes);
typedef void (*gfx_free_fptr)(void* ptr);
typedef void* (*gfx_realloc_fptr)(void* ptr, size_t nbytes);
void gfx_set_mem(gfx_malloc_fptr m, gfx_free_fptr f, gfx_realloc_fptr r);

extern gfx_malloc_fptr gfx_malloc;
extern gfx_free_fptr gfx_free;
extern gfx_realloc_fptr gfx_realloc;

enum gfx_status { GFX_OK = 0,
                  GFX_FAILURE };

enum gfx_status gfx_init();

// ---- Shaders ---------------------------

enum gfx_shader_type {
    GFX_VERTEX_SHADER = GPU_VERTEX_SHADER,
    GFX_FRAGMENT_SHADER = GPU_FRAGMENT_SHADER
};

struct gfx_shader {
    const char* name;
    enum gfx_shader_type type;
    gpu_shader shader;
};

struct gfx_program {
    const char* name;
    gpu_program program;
};

enum gfx_status gfx_create_program(struct gfx_program* program, const char* vs_source, const char* fs_source);
enum gfx_status gfx_create_default_program(struct gfx_program* program);
enum gfx_status gfx_activate_program(struct gfx_program* program);

// ---- Window ----

void gfx_update_window(int x, int y, int width, int height);

// ---- Perspective and view ----

enum gfx_status gfx_update_view(const m4* view, struct gfx_program* program);
enum gfx_status gfx_update_projection(const m4* projection, struct gfx_program* program);

// ---- Mesh ----

struct gfx_mesh_def
{
  uint32_t nverts;
  uint32_t nindices;
  float* positions;  // 3 * nverts
  float* normals;    // 3 * nverts; optional
  float* texcoords;  // 2 * nverts; optional
  uint32_t* indices; // nindices
};

struct gfx_texture_def
{
  uint8_t* data;

  int32_t width;
  int32_t height;
  int32_t ncomps;
};

struct gfx_mesh
{
  struct gpu_vertex_buffer vertex_buffer;
  struct gpu_texture* textures; // owned
  uint32_t ntextures;
};

enum gfx_status gfx_mesh_create(struct gfx_mesh* mesh, const struct gfx_mesh_def*, const struct gfx_texture_def* tex_defs, uint32_t ntexdefs);

void gfx_mesh_destroy(struct gfx_mesh* mesh);

enum gfx_status gfx_mesh_draw(struct gfx_mesh* mesh, const struct gfx_program* program, const m4* transforms, uint32_t ntransforms);

// ---- Geometries ----

void gfx_box_create(struct gfx_mesh_def* out_box);
void gfx_box_destroy(struct gfx_mesh_def* def);

void gfx_circle_create(uint32_t npoints, struct gfx_mesh_def* out_circle);
void gfx_circle_destroy(struct gfx_mesh_def* def);

void gfx_cylinder_create(uint32_t npoints, struct gfx_mesh_def* out_cylinder);
void gfx_cylinder_destroy(struct gfx_mesh_def* def);

// ---- Font ----

struct gfx_glyph {
    uint64_t charcode;
    float advance_x;
    float bearing_x;
    float bearing_y;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
};

struct gfx_font_def {
    uint16_t nglyphs;
    uint16_t bmp_height;
    uint16_t bmp_width;
    float line_spacing;
    uint8_t* bitmap;
    struct gfx_glyph* glyphs;
};

struct gfx_font
{
  struct gpu_texture texture;
  uint16_t nglyphs;
  uint16_t bmp_height;
  uint16_t bmp_width;
  float line_spacing;
  struct gfx_glyph* glyphs;
  bool_t owns_glyphs;
};

enum gfx_status gfx_font_create(struct gfx_font* fnt, const struct gfx_font_def* def, bool_t transfer_glyphs);

void gfx_font_destroy(struct gfx_font* fnt);

// ---- Text ----

struct gfx_text
{
  const char* text_ansi; // not owned
  struct gfx_font* font; // not owned
  struct gpu_vertex_buffer quads;
};

enum gfx_status gfx_text_create(struct gfx_text* txt, const char* text_ansi, struct gfx_font* font );

void gfx_text_destroy(struct gfx_text* txt);

enum gfx_status gfx_text_draw(struct gfx_text* txt, const struct gfx_program* active_program, float screen_x, float screen_y);

// ---- Other ----

void gfx_clear_screen(v3 color);

#ifdef __cplusplus
}
#endif