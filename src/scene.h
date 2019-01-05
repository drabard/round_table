#pragma once

#include <stdint.h>

#include <string>
#include <vector>

#include "drb_math.h"
#include "drb_camera.h"

#include "rendering.h"

struct node {
  v2 position;
  texture_id tex_id;
  std::string name;
};

void node_process_gui(struct node* node);

struct scene {
  struct cam_2d camera;
  std::vector<node> nodes;
};

void scene_load_from_file(struct scene* scene, struct renderer* renderer, struct window* window,
                          const char* path);
void scene_draw(struct scene* scene, struct renderer* renderer);
void scene_process_gui(struct scene* scene, struct renderer* renderer, struct window* window);