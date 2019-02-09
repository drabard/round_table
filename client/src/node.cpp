#include "node.h"

#include <imgui.h>

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