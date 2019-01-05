#include "drb_camera.h"

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

static v3 up = (v3){ .data = { 0.0f, 1.0f, 0.0f } };
static v3 right = (v3){ .data = { 1.0f, 0.0f, 0.0f } };

static void default_log(const char* txt, ...)
{
    va_list argp;
    va_start(argp, txt);
    vprintf(txt, argp);
    va_end(argp);
    fflush(stdout);
}
static cam_log_fptr text_log = default_log;

void cam_set_log(cam_log_fptr l) { text_log = l; }

void cam_noroll_init(struct cam_noroll* cam, v3 position, float yaw,
                     float pitch, const m4* proj_matrix)
{
    cam->position = position;
    cam->orientation = (quat){ .x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f };
    cam->yaw = yaw;
    cam->pitch = pitch;
    cam->projection = *proj_matrix;
    cam_noroll_update(cam, 0.0f, 0.0f, (v3){});
}

void cam_noroll_update(struct cam_noroll* cam, float yaw_d, float pitch_d,
                       v3 pos_d)
{
    cam->yaw += yaw_d;
    cam->pitch += pitch_d;

    quat yaw, pitch;
    quat_from_angle(&yaw, up, cam->yaw);
    quat_from_angle(&pitch, right, cam->pitch);

    quat_mult(&cam->orientation, pitch, yaw);
    quat_norm(&cam->orientation);

    quat_rot(&pos_d, cam->orientation);
    v3_add(&cam->position, pos_d);
}

void cam_noroll_view_projection(struct cam_noroll* cam, m4* out_vp)
{
    m4_view_from_quat(out_vp, cam->orientation, cam->position);
    m4_dot(out_vp, &cam->projection);
}