#include <stdio.h>

#include "window/window.h"

#include "render/render.h"

window_s window;

int main() {
    window = create_window(640, 360, "penguine");
    window_make_context_current(&window);
    printf("Hello, World!\n");
    
    show_window(&window);

    render_init();

    mesh_id_t test_mesh = add_mesh("");
    material_id_t test_material = add_material("");    

    glViewport(0, 0, 640, 360);
    glClearColor(0.0f, 0.05f, 0.1f, 1.0f);

    while (!window_should_close(&window)) {
        window_update(&window);
        glClear(GL_COLOR_BUFFER_BIT);

        draw_mesh(test_mesh, test_material, mat4_identity);
    }

    destroy_window(&window);

    exit(EXIT_SUCCESS);
}