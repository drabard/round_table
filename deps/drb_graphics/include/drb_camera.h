#pragma once

#include "drb_math.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*cam_log_fptr)(const char*, ...);
void cam_set_log(cam_log_fptr l);

struct cam_noroll {
    m4 projection;
    quat orientation;
	v3 position;

    float yaw;
	float pitch;
};

void cam_noroll_init(struct cam_noroll*, v3 position, float yaw,
                     float pitch, const m4* projection);
void cam_noroll_update(struct cam_noroll*, float yaw_d, float pitch_d,
                       v3 pos_d);
void cam_noroll_view_projection(struct cam_noroll*, m4* out_vp);

struct cam_2d {
	m4 projection;
	v2 position;
};

void cam_2d_init(struct cam_2d*, v2 position, float width, float height, float near, float far);
void cam_2d_update(struct cam_2d*, v2 dpos);
void cam_2d_view_projection(struct cam_2d*, m4* out_vp);

#ifdef __cplusplus
}
#endif