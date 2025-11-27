#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "utilities/mathematics.h"

typedef uint32_t collider_id_t;

typedef enum {
    COLLIDER_POINT,
    COLLIDER_SPHERE,
    COLLIDER_AABB
} collider_e;

typedef struct {
    collider_e type;
    bool active;
    void * data;
} collider_s;

typedef struct {
    vec3_t a;
    vec3_t b;
} aabb_collider_s;

typedef struct {
    vec3_t position;
    float radius;
} sphere_collider_s;

typedef vec3_t point_collider_t;

void init_collision();

void update_collisions(double delta_time);

collider_id_t add_collider(collider_s * data);

collider_id_t add_aabb_collider(float * smallest, float * largest);


aabb_collider_s * make_aabb_collider(float * smallest, float * largest);

sphere_collider_s * make_sphere_collider(float * position, float radius);

#define vec3_dist(a, b)\
    sqrtf(powf(a[0], 2) + powf(a[1], 2) + powf(a[2], 2)) - sqrtf(powf(b[0], 2) + powf(b[1], 2) + powf(b[2], 2))

bool vec3_vec3(vec3_t a, vec3_t b);

bool vec3_sphere(vec3_t a, vec3_t b, float r);

bool sphere_sphere(vec3_t a, vec3_t b, float r1, float r2);

bool aabb_aabb();