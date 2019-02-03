#include "node.h"

#include <imgui.h>

void node_process_gui(struct node* node) {
  ImGui::Text("%s: %f, %f", node->name.c_str(), node->position.x,
              node->position.y);
}
