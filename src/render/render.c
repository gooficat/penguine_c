#include "render/render.h"
#include <stdint.h>
#include "render.h"
#include "utilities/file.h"
#include "utilities/mathematics.h"


typedef struct _material {
    vec4_t color;
    float diffuse;
    float specular;
} material_s;

typedef struct _mesh {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLsizei num_indices;
} mesh_s;

static mesh_s *meshes;
static mesh_id_t num_meshes;
static material_s *materials;
static material_id_t num_materials;

static GLuint shader_program;
static GLuint model_uniform_location;
static GLuint view_uniform_location;
static GLuint projection_uniform_location;


static GLuint color_uniform_location;
static GLuint diffuse_uniform_location;
static GLuint specular_uniform_location;

mat4_t view_matrix;
mat4_t projection_matrix;



void render_init(uint32_t width, uint32_t height) {
    gladLoaderLoadGL();
    
    GLuint vertex_shader, fragment_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    
    char * v_src = file_to_string("../res/shader.vert", "r");
    glShaderSource(vertex_shader, 1, &v_src, (void*)0);
    free(v_src);
    glCompileShader(vertex_shader);

    
    char * f_src = file_to_string("../res/shader.frag", "r");
    glShaderSource(fragment_shader, 1, &f_src, (void*)0);
    free(f_src);
    glCompileShader(fragment_shader);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    glUseProgram(shader_program);
    model_uniform_location = glGetUniformLocation(shader_program, "model");
    view_uniform_location = glGetUniformLocation(shader_program, "view");
    projection_uniform_location = glGetUniformLocation(shader_program, "projection");

    color_uniform_location = glGetUniformLocation(shader_program, "color");
    diffuse_uniform_location = glGetUniformLocation(shader_program, "diffuse");
    specular_uniform_location = glGetUniformLocation(shader_program, "specular");

    mat4_copy(view_matrix, mat4_identity);
    mat4_copy(projection_matrix, mat4_identity);

    glUniformMatrix4fv(view_uniform_location, 1, GL_FALSE, &view_matrix[0]);
    glUniformMatrix4fv(projection_uniform_location, 1, GL_FALSE, &projection_matrix[0]);

    glViewport(0, 0, width, height);
}


mesh_id_t add_mesh(const GLfloat *verts, GLsizei num_verts, const GLuint *inds, GLsizei num_inds) {
    mesh_id_t mesh_id = num_meshes++;
    meshes = realloc(meshes, sizeof(mesh_s) * num_meshes);

    mesh_s mesh = {0};
    
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, num_verts * 3 * sizeof(GLfloat), &verts[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_inds * sizeof(GLuint), &inds[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
    glBindVertexArray(0);

    mesh.num_indices = num_inds;

    meshes[mesh_id] = mesh;
    
    return mesh_id;
}

material_id_t add_material(vec4_t color, float diffuse, float specular) {
    material_id_t material_id = num_materials++;
    materials = realloc(materials, sizeof(material_s) * num_materials);
    material_s material = {
        .diffuse = diffuse,
        .specular = specular
    };
    vec4_copy(material.color, color);
    materials[material_id] = material;
    return material_id;
}

void render_clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void draw_mesh(mesh_id_t mesh, material_id_t material, mat4_t transform) {
    glUniformMatrix4fv(model_uniform_location, 1, GL_FALSE, &transform[0]);
    glBindVertexArray(meshes[mesh].vao);
    glUniform4fv(color_uniform_location, 1, materials[material].color);
    
    glUniform1f(diffuse_uniform_location, materials[material].diffuse);
    glUniform1f(specular_uniform_location, materials[material].specular);

    glDrawElements(GL_TRIANGLES, meshes[mesh].num_indices, GL_UNSIGNED_INT, 0);
}

void set_camera_projection(mat4_t matrix) {
    mat4_copy(projection_matrix, matrix);
    glUniformMatrix4fv(projection_uniform_location, 1, GL_FALSE, &projection_matrix[0]);
}

void set_camera_view(mat4_t matrix) {
    mat4_copy(view_matrix, matrix);
    glUniformMatrix4fv(view_uniform_location, 1, GL_FALSE, &view_matrix[0]);

}