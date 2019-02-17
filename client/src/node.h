#pragma once

#include <string>

#include "drb_math.h"

#include "rendering.h"

typedef uint64_t node_id_t;
// unique identifier of a node.

typedef uint32_t node_idx_t;
// nodes are supposed to be stored in an array + freelist data structure.
// this is an index into the array.

// maximum native node types: 2^16 (65536)
// maximum nodes: 2^32 - 1 (enough)
static const node_idx_t INVALID_NODE_IDX = 0xffffffff;
static const node_id_t INVALID_NODE_ID = 0xffffffffffffffff;
static const uint64_t NODE_TYPE_MASK = 0xffffffff00000000;
static const uint32_t NODE_TYPE_SHIFT = 32;
static const uint64_t NODE_IDX_MASK = 0x00000000ffffffff;

#define GET_NODE_TYPE(node_id) (((node_id)&NODE_TYPE_MASK) >> NODE_TYPE_SHIFT)
#define GET_NODE_IDX(node_id) ((node_id)&NODE_IDX_MASK)

enum node_type { SCENE_NODE, SPRITE_NODE };

struct node {
  std::string name;
  v2 position;
  node_id_t id;
  node_id_t parent_id;
  std::vector<node_id_t> children;
  node_idx_t next_free;
  enum node_type type;
};

node_id_t node_build_id(enum node_type type, uint64_t idx);

void node_init(struct node* node, const char* name, v2 position,
               enum node_type type, node_id_t id);
void node_terminate(struct node* node);
void node_change_parent(struct node* node, struct node* old_parent,
                        struct node* new_parent);

void node_gui_properties(struct node* node);

bool node_checkrep(struct node* node);

struct sprite_node {
  struct node node;
  struct sprite sprite;
};
