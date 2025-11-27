#include "collision.h"

bool vec2_vec2(vec2_t a, vec2_t b) {
    return a[0] == b[0] && a[1] == b[1];
}

bool vec3_vec3(vec3_t a, vec3_t b) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

bool vec2_circle(vec2_t a, vec2_t b, float r) {
    return vec2_dist(a, b) < r;
}

bool vec3_sphere(vec3_t a, vec3_t b, float r) {
    return vec3_dist(a, b) < r;
}

bool sphere_sphere(vec3_t a, vec3_t b, float r1, float r2) {
    return vec3_dist(a, b) < r1 + r2;
}