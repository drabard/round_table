#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "drb_camera.h"
#include "drb_gpu.h"
#include "drb_math.h"

typedef uint32_t texture_id;

struct sprite {
  v2 size; // width, height [wu]
  texture_id tex_id;
};

bool sprite_from_image(struct sprite*, struct renderer*, const char* img_path,
                       v2 size);

struct texture_entry {
  struct gpu_texture texture;
  uint32_t refcount;
  texture_id next_free;
};

struct renderer {
  struct gpu_vertex_buffer quad_vb;
  gpu_program program;
  gpu_shader vtx_shader;
  gpu_shader frg_shader;

  std::vector<struct texture_entry> textures;
  texture_id textures_freelist;
  std::unordered_map<std::string, texture_id> textures_lut;
};

bool renderer_init(struct renderer* renderer);
void renderer_terminate(struct renderer* renderer);

void renderer_process_gui(struct renderer* renderer);

texture_id renderer_request_texture(struct renderer* renderer,
                                    const std::string* path);
void renderer_release_texture(struct renderer* renderer, texture_id id);

// todo: batch drawing
void renderer_draw_sprite(struct renderer* renderer, struct sprite* sprite,
                          v2 position, const struct m4* cam_transform);