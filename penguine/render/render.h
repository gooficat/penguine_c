#pragma once
#include "render/gl.h"
#include <stdint.h>

#include "utilities/mathematics.h"

typedef GLuint texture_id_t;
typedef uint32_t mesh_id_t;
typedef uint32_t material_id_t;

void render_init(uint32_t width, uint32_t height);

mesh_id_t add_mesh(const GLfloat *verts, GLsizei num_verts, const GLuint *inds, GLsizei num_inds);

texture_id_t load_texture(const char *filepath);

material_id_t add_material(vec4_t color, float diffuse, texture_id_t texture);

mesh_id_t load_mesh(const char * filepath);

void render_clear();

void draw_mesh(mesh_id_t mesh, material_id_t material, mat4_t transform);

void set_camera_projection(mat4_t matrix);

void set_camera_view(mat4_t matrix);
