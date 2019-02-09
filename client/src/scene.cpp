#include "scene.h"

#include <imgui.h>

#include "drb_camera.h"
#include "file.h"
#include "input.h"
#include "log.h"
#include "rendering.h"
#include "window.h"

void scene_init(struct scene* scene, struct window* window) {
  *scene = (struct scene){};
  cam_2d_init(&scene->camera, (v2){.x = 0.0f, .y = 0.0f}, window->width,
              window->height, 0.0f, 10.0f);
}

bool scene_load_from_file(struct scene* scene, struct renderer* renderer,
                          struct window* window, const char* path) {
  char* buf;
  uint32_t size;
  if (file_load_text(path, &buf, &size) != FILE_OK)
    goto error;

  log_trace(LOG_SCENE, "Read scene file:\n%s", buf);

  return true;

error:
  log_error(LOG_SCENE, "ERROR: Failed to load scene from file.\n");
  return false;
}

struct sprite_node* scene_add_sprite_node(struct scene* scene) {
  scene->sprite_nodes.emplace_back();
  return &scene->sprite_nodes.back();
}

void scene_process_gui(struct scene* scene, struct renderer* renderer,
                       struct window* window) {
  ImGui::Begin("Scene", 0, ImGuiWindowFlags_AlwaysAutoResize);
  {
    if (ImGui::Button("Load from file")) {
      scene_load_from_file(scene, renderer, window,
                           "../scenes/roadside_picnic.json");
    }

    if (ImGui::Button("Add panda")) {
      struct sprite_node* sn = scene_add_sprite_node(scene);
      node_init(&sn->node, "Panda", (v2){}, SPRITE_NODE);
      sprite_from_image(&sn->sprite, renderer, "../data/images/panda.png",
                        (v2){.x = 2.0f, .y = 2.0f});
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
