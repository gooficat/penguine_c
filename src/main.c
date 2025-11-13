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
    set_camera_projection(mat4_ortho(6.4f, 3.6f, 5.0f));

    set_camera_view(mat4_identity);

    mesh_id_t test_mesh = load_mesh("../../../../res/cube.obj");
    material_id_t test_material = add_material(vec4(0.2f, 0.5f, 0.1f, 1.0f), 0.5f);

    float inc = 0.0f;

    while (!window_should_close(&window)) {
        inc += 0.001f;
        render_clear();

        draw_mesh(test_mesh, test_material, mat4_mul(mat4_rotation_y(inc), mat4_rotation_x(inc)));
        window_update(&window);
    }

    destroy_window(&window);

    exit(EXIT_SUCCESS);
}