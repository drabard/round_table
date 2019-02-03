#include "scene.h"

#include <imgui.h>

#include "drb_camera.h"
#include "input.h"
#include "rendering.h"
#include "window.h"

void node_process_gui(struct node* node) {
  ImGui::Text("%s: %f, %f", node->name.c_str(), node->position.x,
              node->position.y);
}

void scene_init(struct scene* scene, struct window* window) {
  *scene = (struct scene){};
  cam_2d_init(&scene->camera, (v2){.x = 0.0f, .y = 0.0f}, window->width,
              window->height, 0.01f, 100.0f);
}

void scene_load_from_file(struct scene* scene, struct renderer* renderer,
                          struct window* window, const char* path) {
  struct sprite spr;
  sprite_from_image(&spr, renderer, "data/images/panda.png",
                    (v2){.x = 2.0f, .y = 2.0f});

  scene->sprite_nodes.push_back(
      (struct sprite_node){.node = (struct node){.name = "Panda",
                                                 .position = (v2){},
                                                 .type = SPRITE_NODE},
                           .sprite = spr});

  cam_2d_init(&scene->camera, (v2){.x = 0.0f, .y = 0.0f}, window->width,
              window->height, 0.0f, 10.0f);
}

void scene_process_gui(struct scene* scene, struct renderer* renderer,
                       struct window* window) {
  ImGui::Begin("Scene", 0, ImGuiWindowFlags_AlwaysAutoResize);
  {
    if (ImGui::Button("Load from file")) {
      scene_load_from_file(scene, renderer, window, "");
    }

    for (uint32_t i = 0; i < scene->sprite_nodes.size(); ++i) {
      ImGui::PushID(i);
      node_process_gui((struct node*)&scene->sprite_nodes[i]);
      ImGui::PopID();
    }
  }
  ImGui::End();
}

void scene_process_input(float dt, struct scene* scene, struct input* input) {
  const float NODE_SPEED = 10.0f;
  for (uint32_t i = 0; i < scene->sprite_nodes.size(); ++i) {
    struct node* n = (struct node*)&scene->sprite_nodes[i];
    if (input->w_pressed)
      n->position.y += dt * NODE_SPEED;
    if (input->s_pressed)
      n->position.y -= dt * NODE_SPEED;
    if (input->d_pressed)
      n->position.x += dt * NODE_SPEED;
    if (input->a_pressed)
      n->position.x -= dt * NODE_SPEED;
  }
}

void scene_draw(struct scene* scene, struct renderer* renderer) {
  m4 cam_view_proj;
  cam_2d_view_projection(&scene->camera, &cam_view_proj);
  for (uint32_t ni = 0; ni < scene->sprite_nodes.size(); ++ni) {
    struct sprite_node* sn = &scene->sprite_nodes[ni];
    renderer_draw_sprite(renderer, &sn->sprite, sn->node.position,
                         &cam_view_proj);
  }
}
