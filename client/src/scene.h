#pragma once

#include <stdint.h>

#include <string>
#include <vector>

#include "drb_camera.h"
#include "drb_math.h"

#include "rendering.h"

#include "node.h"

struct scene {
  struct cam_2d camera;
  std::vector<sprite_node> sprite_nodes;
};

void scene_init(struct scene* scene);
bool scene_load_from_file(struct scene* scene, struct renderer* renderer,
                          struct window* window, const char* path);
void scene_draw(struct scene* scene, struct renderer* renderer);
void scene_process_gui(struct scene* scene, struct renderer* renderer,
                       struct window* window);
void scene_process_input(float dt, struct scene* scene, struct input* input);
