#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "drb_gpu.h"
#include "drb_math.h"

typedef uint32_t texture_id;

struct texture_entry
{
	struct gpu_texture texture;
	uint32_t refcount;
	texture_id next_free;
};

struct renderer
{
	struct gpu_vertex_buffer quad_vb;
	gpu_program program;
	gpu_shader vtx_shader;
	gpu_shader frg_shader;

	std::vector< struct texture_entry > textures;
	texture_id textures_freelist;
	std::unordered_map< std::string, texture_id > textures_lut;
};

void renderer_init(struct renderer* renderer);
void renderer_terminate(struct renderer* renderer);

void renderer_process_gui(struct renderer* renderer);

texture_id renderer_request_texture(struct renderer* renderer, const std::string* path);
void renderer_release_texture(struct renderer* renderer, texture_id id);

void renderer_draw_sprite(texture_id tex_id, v2 position);