#pragma once
#include "render/gl.h"
#include <stdlib.h>

typedef GLuint texture_t;
typedef uint32_t mesh_t;
typedef uint32_t material_t;

typedef struct _material {
    texture_t texture;

} material_s;

typedef struct _mesh {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
} mesh_s;

void render_init();

mesh_t add_mesh(const char * resource_path);

material_t add_material();

void draw_mesh(mesh_t mesh, material_t material);