#include "scene.h"

#include <imgui.h>

#include "drb_camera.h"
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

  scene->nodes.push_back((node){(v2){}, spr, "Panda"});

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

    for (uint32_t i = 0; i < scene->nodes.size(); ++i) {
      ImGui::PushID(i);
      node_process_gui(&scene->nodes[i]);
      ImGui::PopID();
    }
  }
  ImGui::End();
}

void scene_draw(struct scene* scene, struct renderer* renderer) {
  m4 cam_view_proj;
  cam_2d_view_projection(&scene->camera, &cam_view_proj);
  for (uint32_t ni = 0; ni < scene->nodes.size(); ++ni) {
    struct node* node = &scene->nodes[ni];
    renderer_draw_sprite(renderer, &node->sprite, node->position,
                         &cam_view_proj);
  }
}
