#include "drb_imvis.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define ARR_COUNT(arr) (sizeof(arr)/sizeof(arr[0]))

const char* vtx_shader_source = 
	"#version 440\n"
	"layout(location = 0) in vec3 position;"
	"layout(location = 1) uniform mat4 proj_view;"
	"layout(location = 2) uniform mat4 model;"
	"void main()"
	"{"
	"	gl_Position = proj_view * model * vec4(position.xyz, 1.0f);"
	"}";

const char* frg_shader_source =
	"#version 440\n"
	"out vec4 out_color;"
	"uniform vec3 color;"
	"void main()"
	"{"
	"	out_color = vec4(color.xyz, 1.0f);"
	"}";

gpu_program program;
gpu_shader vtx_shader;
gpu_shader frg_shader;
gpu_uniform pv_uniform;
gpu_uniform model_uniform;
gpu_uniform color_uniform;

struct gfx_mesh box;
struct gfx_mesh sphere;

struct draw_info {
	m4 transform;
	v3 color;
};

static struct draw_info box_solid_transforms[IMVIS_MAX_SOLID_BOXES];
static size_t box_solid_counter;

static struct draw_info box_wire_transforms[IMVIS_MAX_WIRE_BOXES];
static size_t box_wire_counter;

static struct draw_info sphere_solid_transforms[IMVIS_MAX_SOLID_SPHERES];
static size_t sphere_solid_counter;

enum imvis_status imvis_init()
{
	gfx_set_mem(malloc, free, realloc);

	if(gpu_compile_shader(&vtx_shader, GPU_VERTEX_SHADER, vtx_shader_source) != GPU_OK) return IMVIS_FAILURE;

	if(gpu_compile_shader(&frg_shader, GPU_FRAGMENT_SHADER, frg_shader_source) != GPU_OK) return IMVIS_FAILURE;

	if(gpu_link_program(&program, vtx_shader, frg_shader) != GPU_OK) return IMVIS_FAILURE;

	gpu_activate_program(program);

	if(gpu_get_uniform(program, "proj_view", &pv_uniform) != GPU_OK) return IMVIS_FAILURE;

	if(gpu_get_uniform(program, "model", &model_uniform) != GPU_OK) return IMVIS_FAILURE;

	if(gpu_get_uniform(program, "color", &color_uniform) != GPU_OK) return IMVIS_FAILURE;

	struct gfx_mesh_def box_def, sphere_def;
	gfx_box_create(&box_def);
	gfx_circle_create(30, &sphere_def);

	if(gfx_mesh_create(&box, &box_def, 0, 0) != GFX_OK) return IMVIS_FAILURE;

	if(gfx_mesh_create(&sphere, &sphere_def, 0, 0) != GFX_OK) return IMVIS_FAILURE;

	gfx_box_destroy(&box_def);
	gfx_circle_destroy(&sphere_def);

	return IMVIS_OK;
}

void imvis_shutdown()
{
	gfx_mesh_destroy(&box);
	gfx_mesh_destroy(&sphere);
	gpu_detach_shader(program, vtx_shader);
	gpu_destroy_shader(vtx_shader);
	gpu_detach_shader(program, frg_shader);
	gpu_destroy_shader(frg_shader);
	gpu_destroy_program(program);
	gpu_activate_program(0);
}

void imvis_add_wire_box(v3 center, v3 extents, v3 color)
{
	if( box_wire_counter >= IMVIS_MAX_WIRE_BOXES ) return;

	struct draw_info* di = &box_wire_transforms[box_wire_counter];

	di->color = color;
	m4_translation(&di->transform, center);
	m4_set_scale(&di->transform, extents);
	box_wire_counter++;
}

void imvis_add_solid_box(v3 center, v3 extents, v3 color)
{
	if( box_solid_counter >= IMVIS_MAX_SOLID_BOXES ) return;

	struct draw_info* di = &box_solid_transforms[box_solid_counter];

	di->color = color;
	m4_translation(&di->transform, center);
	m4_set_scale(&di->transform, extents);
	box_solid_counter++;
}

void imvis_add_solid_sphere(v3 center, float radius, v3 color)
{
	if(sphere_solid_counter >= IMVIS_MAX_SOLID_SPHERES) return;

	struct draw_info* di = &sphere_solid_transforms[sphere_solid_counter];

	di->color = color;
	m4_translation(&di->transform, center);
	m4_set_scale(&di->transform, (v3){ .x = radius, .y = radius, .z = radius});
	sphere_solid_counter++;
}

void imvis_draw(float* proj_view)
{
	gpu_activate_program(program);
	gpu_set_uniform_m4(pv_uniform, proj_view);

	{ // Solid boxes
		for(size_t i = 0; i < box_solid_counter; i++)
		{
			struct draw_info* di = &box_solid_transforms[i];

			gpu_set_uniform_m4(model_uniform, (float*)&di->transform);
			gpu_set_uniform_3f(color_uniform, (float*)&di->color);
			gpu_vertex_buffer_draw(&box.vertex_buffer);
		}
	}

	{ // Wire boxes
		for(size_t i = 0; i < box_wire_counter; i++)
		{
			struct draw_info* di = &box_wire_transforms[i];

			gpu_set_uniform_m4(model_uniform, (float*)&di->transform);
			gpu_set_uniform_3f(color_uniform, (float*)&di->color);
			gpu_vertex_buffer_draw_wireframe(&box.vertex_buffer);
		}
	}

	{ // Solid spheres
		for(size_t i = 0; i < sphere_solid_counter; i++)
		{
			struct draw_info* di = &sphere_solid_transforms[i];

			gpu_set_uniform_m4(model_uniform, (float*)&di->transform);
			gpu_set_uniform_3f(color_uniform, (float*)&di->color);
			gpu_vertex_buffer_draw(&sphere.vertex_buffer);
		}
	}

	box_solid_counter = 0;
	box_wire_counter = 0;
	sphere_solid_counter = 0;
}