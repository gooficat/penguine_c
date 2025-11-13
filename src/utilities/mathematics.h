#pragma once

#include "utilities/mathematics.h"

#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef float vec2_t[2];
typedef float vec3_t[3];
typedef float vec4_t[4];

typedef float mat4_t[16];


#define vec2(x, y) (vec2_t){x, y}
#define vec3(x, y, z) (vec3_t){x, y, z}
#define vec4(x, y, z, w) (vec4_t){x, y, z, w}

extern mat4_t mat4_identity;

#define mat4_copy(a, b) memcpy(a, b, sizeof(mat4_t))
#define vec4_copy(a, b) memcpy(a, b, sizeof(vec4_t))

#define mat4_ortho(r, t, f) (mat4_t){\
    1.0f/r, 0, 0, 0,\
    0, 1.0f/t, 0, 0,\
    0, 0, 1.0f/f, 0,\
    0, 0, 0, 1\
}

#define mat4_translation(x, y, z) (mat4_t){\
    1, 0, 0, 0,\
    0, 1, 0, 0,\
    0, 0, 1, 0,\
    x, y, z, 1\
}

#define mat4_rotation_z(a) (mat4_t){\
    cosf(a), -sinf(a), 0, 0,\
    sinf(a), cosf(a), 0, 0,\
    0, 0, 1, 0,\
    0, 0, 0, 1\
}

#define mat4_rotation_y(a) (mat4_t){\
    cosf(a), 0, -sinf(a), 0,\
    0, 1, 0, 0,\
    sinf(a), 0, cosf(a), 0,\
    0, 0, 0, 1\
}

#define mat4_rotation_x(a) (mat4_t) {\
    1, 0, 0, 0,\
    0, cosf(a), -sinf(a), 0,\
    0, sinf(a), cosf(a), 0,\
    0, 0, 0, 1\
}

#define mat4_mul(a, b) (mat4_t){\
    a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12],\
    \
    a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13],\
    \
    a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14],\
    \
    a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15],\
    \
    \
    a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12],\
    \
    a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13],\
    \
    a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14],\
    \
    a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15],\
    \
    \
    a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12],\
    \
    a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13],\
    \
    a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14],\
    \
    a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15],\
    \
    \
    a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12],\
    \
    a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13],\
    \
    a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14],\
    \
    a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15],\
}