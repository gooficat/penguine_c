#pragma once

#include "utilities/mathematics.h"

#include <string.h>

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