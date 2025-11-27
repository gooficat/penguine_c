#include "render/render.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "render.h"
#include "utilities/file.h"
#include "utilities/mathematics.h"

typedef GLuint texture_t;

typedef struct _material {
    vec4_t color;
    float diffuse;
    texture_t diffuse_texture;
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

static texture_t *textures;
static texture_id_t num_textures;

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
    glBufferData(GL_ARRAY_BUFFER, num_verts * 5 * sizeof(GLfloat), &verts[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_inds * sizeof(GLuint), &inds[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 3));

    mesh.num_indices = num_inds;

    meshes[mesh_id] = mesh;
    
    return mesh_id;
}

material_id_t add_material(vec4_t color, float diffuse, texture_id_t texture) {
    material_id_t material_id = num_materials++;
    materials = realloc(materials, sizeof(material_s) * num_materials);
    material_s material = {
        .diffuse = diffuse,
        .diffuse_texture = texture
    };
    vec4_copy(material.color, color);
    materials[material_id] = material;
    return material_id;
}


mesh_id_t load_mesh(const char * filepath) {
    FILE *f;
    fopen_s(&f, filepath, "rt");
    if (!strcmp(filepath + strlen(filepath) - 4, ".obj")) {

        #define max_len 256 // a line cannot exceed this or it will be cut. for plain obj this is safe except in extreme circumstances
        char buffer[max_len]; // buffer holds line content
        GLfloat *positions = malloc(sizeof(GLfloat) * 3);
        GLfloat *tex_coords = malloc(sizeof(GLfloat) * 3);
        GLsizei num_positions = 0;
        GLsizei num_texcooords = 0;

        GLfloat *vertices = malloc(sizeof(GLfloat) * 5);
        GLsizei num_vertices = 0;

        GLuint *indices = malloc(sizeof(GLuint) * 3);
        GLsizei num_indices = 0;

        while (fgets(buffer, max_len, f)) {
            if (buffer[0] == 'v' && buffer[1] == ' ') {
                    positions = realloc(positions, sizeof(GLfloat) * (num_positions+3));
                    GLfloat *n = &positions[num_positions];
                    num_positions += 3;
                    sscanf_s(buffer, "v %f %f %f", &n[0], &n[1], &n[2]);
            }
            if (buffer[0] == 'v' && buffer[1] == 't') {
                    tex_coords = realloc(tex_coords, sizeof(GLfloat) * (num_texcooords+2));
                    GLfloat *n = &tex_coords[num_texcooords];
                    num_texcooords += 2;
                    sscanf_s(buffer, "vt %f %f", &n[0], &n[1]);
            }
            else if (buffer[0] == 'f') { // if a face
                    GLuint p[3];
                    GLuint t[3];
                    sscanf_s(buffer, "f %d/%d/%*d %d/%d/%*d %d/%d/%*d", &p[0], &t[0], &p[1], &t[1], &p[2], &t[2]);

                    vertices = realloc(vertices, sizeof(GLfloat) * (num_vertices+15));
                    GLfloat *v = &vertices[num_vertices];
                    num_vertices += 15;
                    for (int i = 0; i != 3; ++i) {
                        v[i * 5] = positions[--p[i] * 3];
                        v[i * 5 + 1] = positions[p[i] * 3 + 1];
                        v[i * 5 + 2] = positions[p[i] * 3 + 2];
                        v[i * 5 + 3] = tex_coords[--t[i] * 2];
                        v[i * 5 + 4] = tex_coords[t[i] * 2 + 1];
                    }

                    indices = realloc(indices, sizeof(GLuint) * (num_indices+3));
                    GLuint *n = &indices[num_indices];
                    n[0] = num_indices++;
                    n[1] = num_indices++;
                    n[2] = num_indices++;
            }
        }

        fclose(f);

        mesh_id_t m = add_mesh(vertices, num_vertices, indices, num_indices);

        free(positions);
        free(tex_coords);
        free(indices);
        free(vertices);

        
        return m;
    }

    printf_s("File format of %s not supported", filepath);
    return (mesh_id_t)-1;
}

material_id_t load_material(const char * filepath) {

}

texture_id_t load_texture(const char *filepath) {
    uint8_t header[54];
    uint32_t data_pos;
    uint32_t width, height;
    uint32_t size;

    uint8_t *data;

    FILE *f = NULL;
    fopen_s(&f, filepath, "rb");

    if (!f) {
        printf_s("Error opening texture from path %s", filepath);
        exit(EXIT_FAILURE);
    }

    fread_s(header, sizeof(header), sizeof(uint8_t), 54, f);
    
    data_pos = *(int32_t *) &(header[0x0A]);
    size = *(int32_t *) &(header[0x22]);
    width = *(int32_t *) &(header[0x12]);
    height = *(int32_t *) &(header[0x16]);

    if (!size) {
        size = width * height * 3;
    }
    if (!data_pos) {
        data_pos = 54;
    }

    data = (uint8_t*)malloc(sizeof(uint8_t) * size);

    fread_s(data, size * sizeof(uint8_t), sizeof(uint8_t), size, f);

    if (f) {
        fclose(f);
    }

    texture_id_t texture_id = num_textures++;

    texture_t texture;

    textures = realloc(textures, sizeof(texture_id) * num_textures);

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    glGenerateMipmap(GL_TEXTURE_2D);

    textures[texture_id] = texture;

    free(data);

    return texture_id;
}

void render_clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void draw_mesh(mesh_id_t mesh, material_id_t material, mat4_t transform) {
    glUniformMatrix4fv(model_uniform_location, 1, GL_FALSE, &transform[0]);
    
    glBindTexture(GL_TEXTURE_2D, textures[materials[material].diffuse_texture]);

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