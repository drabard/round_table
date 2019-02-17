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

static std::vector<node_id_t> collect_branch(struct scene* scn, node* node)
// Collects ids of nodes descending from node. Contains node's id as the first
// element.
// Nodes are sorted so that parent nodes are always earlier in the array than
// child nodes.
{
  std::vector<node_id_t> result;
  result.push_back(node->id);

  std::vector<struct node*> buf;
  buf.push_back(node);

  while (!buf.empty()) {
    struct node* cn = buf.back();
    buf.pop_back();

    std::vector<node_id_t>* children = &cn->children;
    for (uint32_t i = 0; i < children->size(); ++i) {
      node_id_t nid = (*children)[i];
      result.push_back(nid);

      struct node* n = scene_get_node_by_id(scn, nid);
      buf.push_back(n);
    }
  }

  return result;
}

void scene_remove_node(struct scene* scn, struct node* node) {
  assert(scn);
  assert(node);

  std::vector<node_id_t> to_remove = collect_branch(scn, node);

  while (!to_remove.empty()) {
    node_id_t id = to_remove.back();
    to_remove.pop_back();

    log_trace(LOG_SCENE, "Removing node (%lu, %lu)\n", GET_NODE_TYPE(id),
              GET_NODE_IDX(id));

    struct node* n = scene_get_node_by_id(scn, id);
    struct node* p = scene_get_node_by_id(scn, n->parent_id);
    node_change_parent(n, p, 0);

    node_idx_t idx = GET_NODE_IDX(n->id);

    node_terminate(n);

    n->next_free = scn->sprite_nodes_freelist;
    scn->sprite_nodes_freelist = idx;
  }
}

struct node* scene_get_node_by_id(struct scene* scene, node_id_t id) {
  if (id == INVALID_NODE_ID)
    return 0;
  if (GET_NODE_IDX(id) >= scene->sprite_nodes.size())
    return 0;

  return (struct node*)&scene->sprite_nodes[GET_NODE_IDX(id)];
}

static void gui_make_tree(struct scene* scene, struct node* node,
                          uint32_t* rec_counter) {
  ++(*rec_counter);
  static const uint32_t MAX_RECURSION_DEPTH = 10;
  if (*rec_counter > MAX_RECURSION_DEPTH) {
    log_error(LOG_SCENE,
              "ERROR: Maximum depth of scene recursion reached for node %s.\n",
              node->name.c_str());
    return;
  }

  bool selected = node->id == scene->gui.selected_node_id;
  bool open = ImGui::TreeNodeEx(
      node->name.c_str(), selected ? ImGuiTreeNodeFlags_Selected
                                   : 0 | ImGuiTreeNodeFlags_OpenOnDoubleClick);

  if (ImGui::IsItemClicked()) {
    scene->gui.selected_node_id = node->id;
  }

  if (open) {
    const std::vector<node_id_t>* children = &node->children;
    for (size_t i = 0; i < children->size(); ++i) {
      struct node* n = scene_get_node_by_id(scene, (*children)[i]);
      if (!n) {
        log_error(LOG_SCENE,
                  "ERROR: Child %lu of node %s can't be found in the scene.\n",
                  i, node->name.c_str());
        continue;
      }

      gui_make_tree(scene, n, rec_counter);
    }

    ImGui::TreePop();
  }

  --(*rec_counter);
}

void scene_process_gui(struct scene* scene, struct renderer* renderer,
                       struct window* window) {
  ImGui::Begin("Scene", 0, ImGuiWindowFlags_AlwaysAutoResize);
  {
    if (ImGui::Button("Load from file")) {
      scene_load_from_file(scene, renderer, window,
                           "../scenes/roadside_picnic.json");
    }

    if (ImGui::Button("Add")) {
      node_id_t nid;
      struct sprite_node* sn = scene_add_sprite_node(scene, &nid);

      struct node* selected_node =
          scene_get_node_by_id(scene, scene->gui.selected_node_id);

      if (sn) {
        node_init(&sn->node, "Panda", (v2){}, SPRITE_NODE, nid);
        node_change_parent(&sn->node, 0, selected_node);
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

      if (sn->node.parent_id == INVALID_NODE_ID) {
        uint32_t rec_counter = 0;
        gui_make_tree(scene, &sn->node, &rec_counter);
      }

#if 0
      if (ImGui::Selectable(sn->node.name.c_str(), selected)) {
        scene->gui.selected_node_id = sn->node.id;
      }
#endif

      ImGui::PopID();
    }
  }
  ImGui::End();

  ImGui::Begin("Node properties");
  {
    struct node* selected_node =
        scene_get_node_by_id(scene, scene->gui.selected_node_id);

    if (selected_node) {
      node_gui_properties(selected_node);
    }
  }
  ImGui::End();

  ImGui::Begin("Scene debug");
  {
    ImGui::Text("sprite nodes allocated: %lu", scene->sprite_nodes.size());
    if (ImGui::TreeNode("Nodes freelist")) {
      node_idx_t idx = scene->sprite_nodes_freelist;
      while (idx != INVALID_NODE_IDX) {
        ImGui::Text("%u", idx);
        idx = scene->sprite_nodes[idx].node.next_free;
      }
      ImGui::TreePop();
    }

    if (ImGui::Button("Dump to log")) {
      for (uint32_t i = 0; i < scene->sprite_nodes.size(); ++i) {
        struct node* n = (struct node*)&scene->sprite_nodes[i];
        log_trace(LOG_SCENE, "\n");
        log_trace(LOG_SCENE, "Node %s\n", n->name.c_str());
        log_trace(LOG_SCENE, "(%lu, %lu), parent (%lu, %lu)\n",
                  GET_NODE_TYPE(n->id), GET_NODE_IDX(n->id),
                  GET_NODE_TYPE(n->parent_id), GET_NODE_IDX(n->parent_id));
        log_trace(LOG_SCENE, "Children: ");
        for (uint32_t ci = 0; ci < n->children.size(); ++ci) {
          node_id_t nid = n->children[ci];
          log_trace(LOG_SCENE, "(%lu, %lu)", GET_NODE_TYPE(nid),
                    GET_NODE_IDX(nid));
          log_trace(LOG_SCENE, ci == n->children.size() - 1 ? "\n" : ", ");
        }
      }
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
