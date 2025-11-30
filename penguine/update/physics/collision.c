#include "update/physics/collision.h"
#include "utilities/memory.h"


void init_collision() {

}

void update_collisions(double delta_time) {

}

bool vec3_vec3(vec3_t a, vec3_t b) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

bool vec3_sphere(vec3_t a, vec3_t b, float r) {
    return vec3_dist(a, b) < r;
}

bool sphere_sphere(vec3_t a, vec3_t b, float r1, float r2) {
    return vec3_dist(a, b) < r1 + r2;
}