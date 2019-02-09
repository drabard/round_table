#pragma once

#include <stdint.h>

#include <string>
#include <vector>

#include "drb_camera.h"
#include "drb_math.h"

#include "rendering.h"

#include "node.h"

struct scene_gui {
  node_id_t selected_node_id;
};

struct scene {
  struct scene_gui gui;
  struct cam_2d camera;
  std::vector<sprite_node> sprite_nodes;
};

void scene_init(struct scene* scene, struct window* window);

bool scene_load_from_file(struct scene* scene, struct renderer* renderer,
                          struct window* window, const char* path);

struct sprite_node* scene_add_sprite_node(struct scene* scene);

struct node* scene_get_node_by_id(struct scene* scene, node_id_t id);

void scene_draw(struct scene* scene, struct renderer* renderer);
void scene_process_gui(struct scene* scene, struct renderer* renderer,
                       struct window* window);
void scene_process_input(float dt, struct scene* scene, struct input* input);
