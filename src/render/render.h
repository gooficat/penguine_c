#pragma once
#include "render/gl.h"
#include <stdint.h>

#include "utilities/mathematics.h"

typedef GLuint texture_id_t;
typedef uint32_t mesh_id_t;
typedef uint32_t material_id_t;

typedef struct _material {
    texture_id_t texture;

} material_s;

typedef struct _mesh {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLsizei num_indices;
} mesh_s;

void render_init();

mesh_id_t add_mesh(const char * resource_path);

material_id_t add_material(const char * resource_path);

void draw_mesh(mesh_id_t mesh, material_id_t material, mat4_t transform);

