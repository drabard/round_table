#pragma once

#include "drb_graphics.h"
#include "drb_math.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IMVIS_MAX_WIRE_BOXES (1 << 20)
#define IMVIS_MAX_SOLID_BOXES (1 << 20)
#define IMVIS_MAX_SOLID_SPHERES (1 << 20)
#define IMVIS_MAX_TRIANGLES (1 << 20)

enum imvis_status {
	IMVIS_OK = 0,
	IMVIS_FAILURE
};

enum imvis_status imvis_init();
void imvis_shutdown();

void imvis_add_wire_box(v3 center, v3 extents, v3 color);
void imvis_add_solid_box(v3 center, v3 extents, v3 color);

void imvis_add_solid_sphere(v3 center, float radius, v3 color);

void imvis_draw(float* proj_view);

#ifdef __cplusplus
}
#endif
