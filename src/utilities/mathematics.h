#pragma once

typedef float vec2_t[2];
typedef float vec3_t[3];
typedef float vec4_t[4];

typedef float mat4_t[16];


#define vec2(x, y) (vec2_t){x, y}
#define vec3(x, y, z) (vec3_t){x, y, z}
#define vec4(x, y, z, w) (vec4_t){x, y, z, w}

#define mat4_identity (mat4_t){\
    1, 0, 0, 0,\
    0, 1, 0, 0,\
    0, 0, 1, 0,\
    0, 0, 0, 1 \
}