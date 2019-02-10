#include "node.h"

#include <assert.h>

#include <imgui.h>

node_id_t node_build_id(enum node_type type, uint64_t idx) {
  assert(idx < 0xffffffffff);
  assert((uint64_t)type < 0xffffff);

  return idx | ((uint64_t)type << NODE_TYPE_SHIFT);
}

void node_init(struct node* node, const char* name, v2 position,
               enum node_type type, node_id_t id) {
  node->name = name;
  node->id = id;
  node->position = position;
  node->type = type;
  node->children = 0;
  node->next_sibling = 0;
}

void node_process_gui(struct node* node) {}