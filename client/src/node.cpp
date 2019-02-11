#include "node.h"

#include <assert.h>

#include <imgui.h>

node_id_t node_build_id(enum node_type type, uint64_t idx) {
  assert(idx < 0xffffffffff);
  assert((uint64_t)type < 0xffffff);

  return idx | ((uint64_t)type << NODE_TYPE_SHIFT);
}

void node_init(struct node* node, const char* name, v2 position,
               enum node_type type, node_id_t id, node_id_t parent_id) {
  node->name = name;
  node->id = id;
  node->parent_id = parent_id;
  node->next_free = INVALID_NODE_IDX;
  node->position = position;
  node->type = type;
}

void node_terminate(struct node* node) {
  *node = (struct node){};
  node->id = INVALID_NODE_ID;
  node->parent_id = INVALID_NODE_ID;
  node->next_free = INVALID_NODE_IDX;
}

void node_gui_properties(struct node* n) {
  ImGui::Text("%s (%lu, %lu)", n->name.c_str(), GET_NODE_TYPE(n->id),
              GET_NODE_IDX(n->id));
  if (n->parent_id == INVALID_NODE_ID) {
    ImGui::Text("Orphan");
  } else {
    ImGui::Text("Parent: (%lu, %lu)", GET_NODE_TYPE(n->parent_id),
                GET_NODE_IDX(n->parent_id));
  }
  ImGui::InputFloat2("position", (float*)&n->position);
}