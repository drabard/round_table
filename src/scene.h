#pragma once

#include <stdint.h>

#include <vector>
#include <string>

#include "drb_math.h"

#include "rendering.h"

struct node
{
	v2 position;
	texture_id tex_id;
	std::string name;
};

void node_process_gui(struct node* node);

struct scene
{
	std::vector< node > nodes;
};

void scene_load_from_file(struct scene* scene, struct renderer* renderer, const char* path);
void scene_draw(struct scene* scene, struct renderer* renderer);
void scene_process_gui(struct scene* scene, struct renderer* renderer);