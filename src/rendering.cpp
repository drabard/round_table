#include "rendering.h"

#include <imgui.h>

#include <cassert>

#include "stb_image.h"

static const texture_id INVALID_TID = 0xffffffff;

static const char* frg_shader_src = "#version 440\n"
	"out vec4 color;"
	"in vec3 pos;"
	"in vec3 norm;"
	"in vec2 tex;"
	"uniform vec3 light_pos;"
	"uniform sampler2D tex0;"
	"uniform mat4 model;"
	"void main()"
	"{"
	"	color = vec4(texture(tex0, tex).xyz, 1.0);"
	"}";

static const char* vtx_shader_src = "#version 440\n"
	"layout(location = 0) in vec3 position;"
	"layout(location = 1) in vec3 normal;"
	"layout(location = 2) in vec3 texcoord;"
	"uniform mat4 model;"
	"uniform mat4 view;"
	"uniform mat4 projection;"
	"out vec3 pos;"
	"out vec3 norm;"
	"out vec2 tex;"
	"void main()"
	"{"
	"  pos = position;"
	"  norm = normal;"
	"  tex = texcoord.xy;"
	"  gl_Position = projection*view*model*vec4(position.xyz, 1.0f);"
	"}";

static float quad_vertices[] = {-0.5, -0.5, 0.0f, 0.0f, 0.0f,
    0.5, -0.5, 0.0f, 1.0f, 0.0f,
    0.5, 0.5, 0.0f, 1.0f, 1.0f,
    -0.5, 0.5, 0.0f, 0.0f, 1.0f
};

static uint32_t quad_indices[] = {
    0, 1, 2,
    2, 3, 0
};

void renderer_init(struct renderer* renderer)
{
	*renderer = (struct renderer){};
	renderer->textures_freelist = INVALID_TID;

	gpu_vertex_buffer_create(&renderer->quad_vb, quad_vertices, GPU_POS | GPU_TEXCOORD, quad_indices, sizeof(quad_vertices)/(sizeof(quad_vertices[0]) * 5), sizeof(quad_indices)/sizeof(quad_indices[0]));

	gpu_compile_shader(&renderer->vtx_shader, GL_VERTEX_SHADER,
	                                   vtx_shader_src);
	gpu_compile_shader(&renderer->frg_shader, GL_FRAGMENT_SHADER,
	                                   frg_shader_src);
	gpu_link_program(&renderer->program, renderer->vtx_shader,
	                                 renderer->frg_shader);
	gpu_activate_program(renderer->program);
}

void renderer_terminate(struct renderer* renderer)
{
	for(uint32_t ti = 0; ti < renderer->textures.size(); ++ti)
	{
		gpu_texture_destroy(&renderer->textures[ti].texture);
	}

	gpu_vertex_buffer_destroy(&renderer->quad_vb);

	*renderer = (struct renderer){};
	renderer->textures_freelist = INVALID_TID;
}

texture_id renderer_request_texture(struct renderer* renderer, const std::string* path)
{
	auto it = renderer->textures_lut.find(*path);
	if(it != std::end(renderer->textures_lut))
	{
		texture_id tid = it->second;
		texture_entry* te = &renderer->textures[tid];
		te->refcount++;
		return tid;
	}

	struct texture_entry new_entry;

	int width = 0;
	int height = 0;
	int comp = 0;
	const uint8_t* data = stbi_load(path->c_str(), &width, &height, &comp, 3);
	gpu_texture_create_RGBA(&new_entry.texture, data, width, height);

	new_entry.next_free = INVALID_TID;
	new_entry.refcount = 1;

	renderer->textures.emplace_back(std::move(new_entry));

	renderer->textures_lut[*path] = renderer->textures.size() - 1;

	return renderer->textures.size() - 1;
}

void renderer_release_texture(struct renderer* renderer, texture_id id)
{
	texture_entry* te = &renderer->textures[id];

	assert(te->refcount > 0);

	te->refcount--;
	if(te->refcount == 0)
	{
		gpu_texture_destroy(&te->texture);

		*te = (struct texture_entry){};

		te->next_free = renderer->textures_freelist;
		renderer->textures_freelist = id;
	}
}

void renderer_draw_sprite(struct renderer* renderer, texture_id tex_id, v2 position)
{
//void gpu_vertex_buffer_draw(const struct gpu_vertex_buffer* tgt);

	gpu_vertex_buffer_draw(&renderer->quad_vb);
}

void renderer_process_gui(struct renderer* renderer)
{
	ImGui::Begin("Renderer debug", 0, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Quad VB: %u, %u, %u, %u", renderer->quad_vb.vao, renderer->quad_vb.vertex_buf, renderer->quad_vb.elem_buf, renderer->quad_vb.nindices);
	ImGui::Text("Loaded textures: %zu", renderer->textures.size());
	ImGui::End();
}
