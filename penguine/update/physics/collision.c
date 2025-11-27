#include "collision.h"
#include "utilities/memory.h"

static collider_s * * colliders;
static collider_id_t num_colliders = 0;

void init_collision() {
    colliders = malloc(sizeof(collider_s *));
}

void update_collisions(double delta_time) {
    for (int i = 0; i != num_colliders; ++i) {
        switch (colliders[i]->type) {
            case COLLIDER_AABB: {
                for (int j = 0; j != num_colliders; ++j) {
                    switch (colliders[j]->type) {
                        case COLLIDER_AABB: {
                            
                        }
                        case COLLIDER_POINT: {

                        }
                        case COLLIDER_SPHERE: {

                        }
                    }
                }
            }
            case COLLIDER_POINT: {

            }
            case COLLIDER_SPHERE: {

            }
        }
    }
}

collider_id_t add_collider(collider_s * data) {
    collider_id_t collider_id = num_colliders++;
    safe_realloc(colliders, sizeof(collider_s *) * num_colliders);

    colliders[collider_id] = data;

    return collider_id;
}

collider_id_t add_aabb_collider(float * smallest, float * largest) {
    return add_collider(make_aabb_collider(smallest, largest));
}

aabb_collider_s * make_aabb_collider(float * smallest, float * largest) {
    aabb_collider_s * collider = malloc(sizeof(aabb_collider_s));
    *collider = (aabb_collider_s) {0};
    memcpy_s(collider->a, sizeof(vec3_t), smallest, sizeof(vec3_t));
    memcpy_s(collider->b, sizeof(vec3_t), largest, sizeof(vec3_t));

    return collider;
}

sphere_collider_s * make_sphere_collider(float * position, float radius) {

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