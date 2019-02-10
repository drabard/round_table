#pragma once

#include <string>

#include "drb_math.h"

#include "rendering.h"

typedef uint64_t node_id_t;

// maximum native node types: 2^16 (65536)
// maximum nodes: 2^40 (enough)
static const node_id_t INVALID_NODE_ID = 0xffffffffffffffff;
static const uint64_t NODE_TYPE_MASK = 0xffffff0000000000;
static const uint32_t NODE_TYPE_SHIFT = 40;
static const uint64_t NODE_IDX_MASK = 0x000000ffffffffff;

#define GET_NODE_TYPE(node_id) (((node_id)&NODE_TYPE_MASK) >> NODE_TYPE_SHIFT)
#define GET_NODE_IDX(node_id) ((node_id)&NODE_IDX_MASK)

enum node_type { SCENE_NODE, SPRITE_NODE };

struct node {
  std::string name;
  v2 position;
  node_id_t id;
  struct node* children;
  struct node* next_sibling;
  enum node_type type;
};

node_id_t node_build_id(enum node_type type, uint64_t idx);

void node_init(struct node* node, const char* name, v2 position,
               enum node_type type, node_id_t id);
void node_process_gui(struct node* node);

struct sprite_node {
  struct node node;
  struct sprite sprite;
};
