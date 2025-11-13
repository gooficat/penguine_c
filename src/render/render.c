#include "render/render.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "render.h"
#include "utilities/file.h"
#include "utilities/mathematics.h"


typedef struct _material {
    vec4_t color;
    float diffuse;
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

mat4_t view_matrix;
mat4_t projection_matrix;



void render_init(uint32_t width, uint32_t height) {
    gladLoaderLoadGL();
    
    GLuint vertex_shader, fragment_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    
    char * v_src = file_to_string("../../../../res/shader.vert", "r");
    glShaderSource(vertex_shader, 1, &v_src, (void*)0);
    free(v_src);
    glCompileShader(vertex_shader);

    
    char * f_src = file_to_string("../../../../res/shader.frag", "r");
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

    mat4_copy(view_matrix, mat4_identity);
    mat4_copy(projection_matrix, mat4_identity);

    glUniformMatrix4fv(view_uniform_location, 1, GL_FALSE, &view_matrix[0]);
    glUniformMatrix4fv(projection_uniform_location, 1, GL_FALSE, &projection_matrix[0]);

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
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

    mesh.num_indices = num_inds;

    meshes[mesh_id] = mesh;
    
    return mesh_id;
}

material_id_t add_material(vec4_t color, float diffuse) {
    material_id_t material_id = num_materials++;
    materials = realloc(materials, sizeof(material_s) * num_materials);
    material_s material = {
        .diffuse = diffuse,
    };
    vec4_copy(material.color, color);
    materials[material_id] = material;
    return material_id;
}


mesh_id_t load_mesh(const char * filepath) {
    FILE *f;
    fopen_s(&f, filepath, "rt");
    if (filepath[strlen(filepath)-3] == 'o') { // obj

        #define max_len 256 // a line cannot exceed this or it will be cut. for plain obj this is safe except in extreme circumstances
        char buffer[max_len]; // buffer holds line content
        GLfloat *positions = malloc(sizeof(GLfloat) * 3);
        GLsizei num_positions = 0;
        GLuint *indices = malloc(sizeof(GLuint) * 3);
        GLsizei num_indices = 0;

        while (fgets(buffer, max_len, f)) {
            if (buffer[0] == 'v' && buffer[1] == ' ') {
                    positions = realloc(positions, sizeof(GLfloat) * (num_positions+3));
                    GLfloat *n = &positions[num_positions];
                    num_positions += 3;
                    sscanf_s(buffer, "v %f %f %f", &n[0], &n[1], &n[2]);
            }
            else if (buffer[0] == 'f') { // if a face
                    indices = realloc(indices, sizeof(GLuint) * (num_indices+3));
                    GLuint *i = &indices[num_indices];
                    num_indices += 3;
                    sscanf_s(buffer, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &i[0], &i[1], &i[2]);
                    --i[0];
                    --i[1];
                    --i[2];
            }
        }

        fclose(f);

        mesh_id_t m = add_mesh(positions, num_positions, indices, num_indices);

        free(positions);
        free(indices);
        
        return m;
    }

    printf_s("File format of %s not supported", filepath);
    return (mesh_id_t)-1;
}

material_id_t load_material(const char * filepath) {

}

void render_clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void draw_mesh(mesh_id_t mesh, material_id_t material, mat4_t transform) {
    glUniformMatrix4fv(model_uniform_location, 1, GL_FALSE, &transform[0]);
    
    glBindVertexArray(meshes[mesh].vao);

    glUniform4fv(color_uniform_location, 1, materials[material].color);
    glUniform1f(diffuse_uniform_location, materials[material].diffuse);

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