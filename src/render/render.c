#include "render/render.h"
#include "render.h"
#include "utilities/file.h"
#include "utilities/mathematics.h"

static mesh_s *meshes;
static mesh_id_t num_meshes;
static material_s *materials;
static material_id_t num_materials;

static GLuint shader_program;
static GLuint model_uniform_location;
static GLuint view_uniform_location;
static GLuint projection_uniform_location;

mat4_t view_matrix;
mat4_t projection_matrix;



void render_init() {
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

    mat4_copy(view_matrix, mat4_identity);
    mat4_copy(projection_matrix, mat4_identity);

    glUniformMatrix4fv(view_uniform_location, 1, GL_FALSE, &view_matrix[0]);
    glUniformMatrix4fv(projection_uniform_location, 1, GL_FALSE, &projection_matrix[0]);
}


mesh_id_t add_mesh(const char * resource_path) {
    mesh_id_t mesh_id = num_meshes++;
    meshes = realloc(meshes, sizeof(mesh_s) * num_meshes);

    GLfloat verts[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0,
        0.5f, -0.5f, 0.0f
    };
    GLsizei num_verts = 3;

    GLuint inds[] = {0, 1, 2};
    GLsizei num_inds = 3;

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

material_id_t add_material(const char * resource_path) {
    material_id_t material_id = num_materials++;
    materials = realloc(materials, sizeof(material_s) * num_materials);

    return material_id;
}

void draw_mesh(mesh_id_t mesh, material_id_t material, mat4_t transform) {
    glUniformMatrix4fv(model_uniform_location, 1, GL_FALSE, &transform[0]);
    glBindVertexArray(meshes[mesh].vao);
    glDrawElements(GL_TRIANGLES, meshes[mesh].num_indices, GL_UNSIGNED_INT, 0);
}