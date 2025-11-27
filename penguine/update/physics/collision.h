#pragma once
#include <stdbool.h>

#include "utilities/mathematics.h"

#define vec2_dist(a, b)\
    sqrtf(powf(a[0], 2) + powf(a[1], 2)) - sqrtf(powf(b[0], 2) + powf(b[1], 2))

#define vec3_dist(a, b)\
    sqrtf(powf(a[0], 2) + powf(a[1], 2) + powf(a[2], 2)) - sqrtf(powf(b[0], 2) + powf(b[1], 2) + powf(b[2], 2))

bool vec2_vec2(vec2_t a, vec2_t b);
bool vec3_vec3(vec3_t a, vec3_t b);

bool vec2_circle(vec2_t a, vec2_t b, float r);
bool vec3_sphere(vec3_t a, vec3_t b, float r);

bool sphere_sphere(vec3_t a, vec3_t b, float r1, float r2);
