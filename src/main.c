#include <stdio.h>

#include "utilities/mathematics.h"
#include "window/window.h"

#include "render/render.h"

window_s window;

int main() {
    window = create_window(640, 360, "penguine");
    window_make_context_current(&window);
    printf("Hello, World!\n");
    
    show_window(&window);

    render_init(640, 360);
    set_camera_projection(mat4_ortho(6.4f, 3.6f, 1.0f));

    set_camera_view(mat4_identity);

    mesh_id_t test_mesh;
    material_id_t test_material;
    
    test_mesh = add_mesh((float[]){
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0
    }, 3, (uint32_t[]){0, 1, 2}, 3);
    test_material = add_material(0.5f, 1.0f);


    while (!window_should_close(&window)) {
        window_update(&window);
        render_clear();

        draw_mesh(test_mesh, test_material, mat4_identity);
    }

    destroy_window(&window);

    exit(EXIT_SUCCESS);
}