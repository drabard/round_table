#include "scene.h"

#include <assert.h>

#include <imgui.h>

#include "drb_camera.h"
#include "file.h"
#include "input.h"
#include "log.h"
#include "rendering.h"
#include "window.h"

void scene_init(struct scene* scene, struct window* window) {
  *scene = (struct scene){};
  scene->gui.selected_node_id = INVALID_NODE_ID;
  scene->sprite_nodes_freelist = INVALID_NODE_IDX;
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

struct sprite_node* scene_add_sprite_node(struct scene* scene,
                                          node_id_t* out_id) {
  struct sprite_node* res = 0;
  node_id_t new_id = INVALID_NODE_ID;
  node_idx_t new_idx = INVALID_NODE_IDX;

  if (out_id == 0)
    goto error;

  if (scene->sprite_nodes_freelist != INVALID_NODE_IDX) {
    res = &scene->sprite_nodes[scene->sprite_nodes_freelist];
    new_idx = scene->sprite_nodes_freelist;
    scene->sprite_nodes_freelist = res->node.next_free;
  } else {
    scene->sprite_nodes.emplace_back();
    res = &scene->sprite_nodes.back();
    new_idx = scene->sprite_nodes.size() - 1;
  }

  new_id = node_build_id(SPRITE_NODE, new_idx);

  *out_id = new_id;

  return res;

error:
  log_error(LOG_SCENE, "ERROR: Failed to add a sprite node.\n");
  return 0;
}

void scene_remove_node(struct scene* scn, struct node* node) {
  assert(node);
  assert(node->type == SPRITE_NODE); // other types not implemented yet

  node_idx_t idx = GET_NODE_IDX(node->id);

  node_terminate(node);

  node->next_free = scn->sprite_nodes_freelist;
  scn->sprite_nodes_freelist = idx;
}

struct node* scene_get_node_by_id(struct scene* scene, node_id_t id) {
  if (id == INVALID_NODE_ID)
    return 0;
  if (GET_NODE_IDX(id) >= scene->sprite_nodes.size())
    return 0;

  return (struct node*)&scene->sprite_nodes[GET_NODE_IDX(id)];
}

void scene_process_gui(struct scene* scene, struct renderer* renderer,
                       struct window* window) {

  struct node* selected_node =
      scene_get_node_by_id(scene, scene->gui.selected_node_id);
  ImGui::Begin("Scene", 0, ImGuiWindowFlags_AlwaysAutoResize);
  {
    if (ImGui::Button("Load from file")) {
      scene_load_from_file(scene, renderer, window,
                           "../scenes/roadside_picnic.json");
    }

    if (ImGui::Button("Add")) {
      node_id_t nid;
      struct sprite_node* sn = scene_add_sprite_node(scene, &nid);
      if (sn) {
        node_init(&sn->node, "Panda", (v2){}, SPRITE_NODE, nid,
                  scene->gui.selected_node_id);
        sprite_from_image(&sn->sprite, renderer, "../data/images/panda.png",
                          (v2){.x = 2.0f, .y = 2.0f});
      }
    }

    if (ImGui::Button("Remove")) {
      if (scene->gui.selected_node_id != INVALID_NODE_ID) {
        node_id_t id = scene->gui.selected_node_id;
        struct node* n = scene_get_node_by_id(scene, id);
        if (n) {
          scene_remove_node(scene, n);
          scene->gui.selected_node_id = INVALID_NODE_ID;
        }
      }
    }

    ImGui::Separator();

    for (uint32_t i = 0; i < scene->sprite_nodes.size(); ++i) {
      ImGui::PushID(i);

      struct sprite_node* sn = &scene->sprite_nodes[i];

      if (sn->node.id == INVALID_NODE_ID) {
        ImGui::PopID();
        continue;
      }

      bool selected = sn->node.id == scene->gui.selected_node_id;
      if (ImGui::Selectable(sn->node.name.c_str(), selected)) {
        scene->gui.selected_node_id = sn->node.id;
      }

      ImGui::PopID();
    }
  }
  ImGui::End();

  ImGui::Begin("Node properties");
  {
    if (selected_node) {
      node_gui_properties(selected_node);
    }
  }
  ImGui::End();

  ImGui::Begin("Scene debug");
  {
    ImGui::Text("sprite nodes: %lu", scene->sprite_nodes.size());
    if (scene->sprite_nodes_freelist == INVALID_NODE_IDX) {
      ImGui::Text("sprite nodes freelist empty");
    } else {
      ImGui::Text("sprite nodes freelist: %u", scene->sprite_nodes_freelist);
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
    if (sn->node.id == INVALID_NODE_ID)
      continue;

    renderer_draw_sprite(renderer, &sn->sprite, sn->node.position,
                         &cam_view_proj);
  }
}
