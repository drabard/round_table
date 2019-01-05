#include "rendering.h"

#include <imgui.h>

#include <cassert>
#include <cstdio>

#include "stb_image.h"

static const texture_id INVALID_TID = 0xffffffff;

static const char* frg_shader_src =
    "#version 440\n"
    "out vec4 color;"
    "in vec3 pos;"
    "in vec3 norm;"
    "in vec2 tex;"
    "uniform sampler2D tex0;"
    "uniform mat4 model;"
    "void main()"
    "{"
    "	color = vec4(texture(tex0, tex).xyz, 1.0);"
    "}";

static const char* vtx_shader_src =
    "#version 440\n"
    "layout(location = 0) in vec3 position;"
    "layout(location = 1) in vec3 normal;"
    "layout(location = 2) in vec3 texcoord;"
    "uniform mat4 model;"
    "uniform mat4 view_projection;"
    "out vec3 pos;"
    "out vec3 norm;"
    "out vec2 tex;"
    "void main()"
    "{"
    "  pos = position;"
    "  norm = normal;"
    "  tex = texcoord.xy;"
    "  gl_Position = view_projection*model*vec4(position.xyz, 1.0f);"
    "  gl_Position = model*vec4(position.xyz, 1.0f);"    
    "}";

static const char* model_uni_name = "model";
static const char* view_projection_uni_name = "view_projection";

static const char* tex_sampler_name = "tex0";

static float quad_vertices[] = {-0.5, -0.5, 0.0f, 0.0f, 0.0f, 0.5,  -0.5,
                                0.0f, 1.0f, 0.0f, 0.5,  0.5,  0.0f, 1.0f,
                                1.0f, -0.5, 0.5,  0.0f, 0.0f, 1.0f};

static uint32_t quad_indices[] = {0, 1, 2, 2, 3, 0};

bool renderer_init(struct renderer* renderer) {
  *renderer = (struct renderer){};
  renderer->textures_freelist = INVALID_TID;

  gpu_vertex_buffer_create(
      &renderer->quad_vb, quad_vertices, GPU_POS | GPU_TEXCOORD, quad_indices,
      sizeof(quad_vertices) / (sizeof(quad_vertices[0]) * 5),
      sizeof(quad_indices) / sizeof(quad_indices[0]));

  gpu_compile_shader(&renderer->vtx_shader, GL_VERTEX_SHADER, vtx_shader_src);
  gpu_compile_shader(&renderer->frg_shader, GL_FRAGMENT_SHADER, frg_shader_src);
  gpu_link_program(&renderer->program, renderer->vtx_shader,
                   renderer->frg_shader);
  gpu_activate_program(renderer->program);

  m4 identity;
  m4_unit(&identity);

  gpu_uniform uni;
  if (gpu_get_uniform(renderer->program, model_uni_name, &uni) != GPU_OK)
    goto error;
  if (gpu_set_uniform_m4(uni, (float*)identity.data) != GPU_OK)
    goto error;

  stbi_set_flip_vertically_on_load(1);

  return true;

error:
  fprintf(stderr, "ERROR: Failed to initialize renderer.\n");
  return false;
}

void renderer_terminate(struct renderer* renderer) {
  for (uint32_t ti = 0; ti < renderer->textures.size(); ++ti) {
    gpu_texture_destroy(&renderer->textures[ti].texture);
  }

  gpu_vertex_buffer_destroy(&renderer->quad_vb);

  *renderer = (struct renderer){};
  renderer->textures_freelist = INVALID_TID;
}

texture_id renderer_request_texture(struct renderer* renderer,
                                    const std::string* path) {
  auto it = renderer->textures_lut.find(*path);
  if (it != std::end(renderer->textures_lut)) {
    texture_id tid = it->second;
    texture_entry* te = &renderer->textures[tid];
    te->refcount++;
    return tid;
  }

  struct texture_entry new_entry;

  int width = 0;
  int height = 0;
  int comp = 0;
  const uint8_t* data = stbi_load(path->c_str(), &width, &height, &comp, 4);
  gpu_texture_create_RGBA(&new_entry.texture, data, width, height);
  stbi_image_free((void*)data);

  new_entry.next_free = INVALID_TID;
  new_entry.refcount = 1;

  renderer->textures.emplace_back(std::move(new_entry));

  renderer->textures_lut[*path] = renderer->textures.size() - 1;

  return renderer->textures.size() - 1;
}

void renderer_release_texture(struct renderer* renderer, texture_id id) {
  texture_entry* te = &renderer->textures[id];

  assert(te->refcount > 0);

  te->refcount--;
  if (te->refcount == 0) {
    gpu_texture_destroy(&te->texture);

    *te = (struct texture_entry){};

    te->next_free = renderer->textures_freelist;
    renderer->textures_freelist = id;
  }
}

void renderer_draw_sprite(struct renderer* renderer, const m4* pv, texture_id tex_id,
                          v2 position) {
  gpu_activate_program(renderer->program);

  m4 pvt;
  m4_unit(&pvt);

  gpu_uniform uni;
  if (gpu_get_uniform(renderer->program, view_projection_uni_name, &uni) != GPU_OK)
    goto error;
  if (gpu_set_uniform_m4(uni, (float*)&pvt) != GPU_OK)
    goto error;

  gpu_texture_bind(&renderer->textures[tex_id].texture, &renderer->program, 1,
                   0, tex_sampler_name);
  gpu_vertex_buffer_draw(&renderer->quad_vb);

  return;

  error:
  fprintf(stderr, "ERROR: Failed to draw sprite.\n");
}

void renderer_process_gui(struct renderer* renderer) {
  ImGui::Begin("Renderer debug", 0, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("Quad VB: %u, %u, %u, %u", renderer->quad_vb.vao,
              renderer->quad_vb.vertex_buf, renderer->quad_vb.elem_buf,
              renderer->quad_vb.nindices);
  ImGui::Text("Loaded textures: %zu", renderer->textures.size());
  ImGui::End();
}
