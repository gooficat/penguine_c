#include "render/render.h"

static mesh_s *meshes;
static mesh_t num_meshes;
static material_s *material;
static material_t num_materials;

void render_init() {
    load_gl();
    
}


mesh_t add_mesh(const char * resource_path) {
    mesh_t mesh_id = num_meshes;
    meshes = realloc(meshes, sizeof(mesh_s) * ++num_meshes);
    // glGenBuffers(1, &meshes[mesh_id].vbo);
    return mesh_id;
}

material_t add_material(const char * resource_path) {

}

void draw_mesh(mesh_t mesh, material_t material) {

}