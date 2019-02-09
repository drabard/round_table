#pragma once

#include <string>

#include "drb_math.h"

#include "rendering.h"

typedef uint32_t node_id_t;
static const node_id_t INVALID_NODE_ID = 0xffffffff;

enum node_type { SCENE_NODE, SPRITE_NODE };

struct node {
  std::string name;
  v2 position;
  node_id_t id;
  struct node* children;
  struct node* next_sibling;
  enum node_type type;
};

void node_init(struct node* node, const char* name, v2 position,
               enum node_type type, node_id_t id);
void node_process_gui(struct node* node);

struct sprite_node {
  struct node node;
  struct sprite sprite;
};
