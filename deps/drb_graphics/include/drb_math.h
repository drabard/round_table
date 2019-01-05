#pragma once

#include <math.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct v2 {
    union {
        float data[2];
        struct {
            float x;
            float y;
        };
    };
} v2;

void v2_copy(v2* dst, v2 src);
void v2_neg(v2*);
void v2_add(v2* dst, v2 src);
void v2_sub(v2* dst, v2 src);
float v2_norm(v2*);
void v2_mult(v2*, float s);
void v2_cross(v2* dst, v2 src);
float v2_len(v2);
float v2_len_sq(v2);
float v2_dot(v2, v2);

typedef struct v3 {
    union {
        float data[3];
        struct {
            float x;
            float y;
            float z;
        };
    };
} v3;

void v3_copy(v3* dst, v3 src);
void v3_neg(v3*);
void v3_add(v3* dst, v3 src);
void v3_sub(v3* dst, v3 src);
float v3_norm(v3*);
void v3_mult(v3*, float s);
void v3_cross(v3* dst, v3 src);
float v3_len(v3);
float v3_len_sq(v3);
float v3_dot(v3, v3);

typedef struct v4 {
    union {
        float data[4];
        struct {
            float x;
            float y;
            float z;
            float w;
        };
    };
} v4;

void v4_copy(v4* dst, v4 src);
void v4_neg(v4*);
void v4_add(v4* dst, v4 src);
void v4_sub(v4* dst, v4 src);
float v4_norm(v4*);
void v4_mult(v4*, float s);
void v4_cross(v4* dst, v4 src);

float v4_len(v4);
float v4_len_sq(v4);
float v4_dot(v4, v4);

typedef struct quat {
    union {
        float data[4];
        struct {
            float x;
            float y;
            float z;
            float w;
        };
    };
} quat;

quat quat_unit();
void quat_from_angle(quat*, v3 axis, float angle);
void quat_copy(quat*, quat src);
void quat_norm(quat*);
void quat_conj(quat*);
void quat_mult(quat* res, quat left, quat right);
void quat_rot(v3*, quat);

float quat_len(quat);
float quat_len_sq(quat);

typedef struct m4 {
    float data[16];
} m4;

#define m4_at(matrix, row, column) (&(matrix)->data[4 * (row) + (column)])

void m4_copy(m4*, const m4* src);
void m4_unit(m4*);
void m4_dot(m4*, const m4* right);
void m4_inverse(m4*);
void m4_transform(m4, v3*);
void m4_from_quat(m4*, quat);

void m4_rotation(m4*, v3 angles);
void m4_translation(m4*, v3);
void m4_set_translation(m4*, v3);
void m4_scale(m4*, v3);
void m4_set_scale(m4*, v3);
void m4_look_at(m4*, v3 from, v3 to, v3 up);
void m4_orthographic(m4* dst, float left, float right, float top, float bottom, float near, float far);
void m4_orthographic_sym(m4* dst, float width, float aspect_ratio, float near, float far);
void m4_perspective(m4*, float fov_deg, float aspect, float near_plane,
                    float far_plane);
void m4_view_from_quat(m4*, quat q, v3 pos);

#ifdef __cplusplus
}
#endif
