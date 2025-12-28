#include <stdio.h>

#include "penguine.h"

window_s window;

int main(void)
{
    window = create_window(640, 360, "penguine");
    window_make_context_current(&window);

    show_window(&window);

    render_init(640, 360);
    set_camera_projection(mat4_ortho(6.4f, 3.6f, 5.0f));

    set_camera_view(mat4_identity);

    texture_id_t test_texture = load_texture("./res/tiles.bmp");
    mesh_id_t test_mesh = load_mesh("./res/cube.obj");

    material_id_t test_material = add_material(vec4(0.2f, 0.5f, 0.1f, 1.0f), 0.5f, test_texture);

    while (!window_should_close(&window))
    {
        render_clear();

        draw_mesh(test_mesh, test_material, mat4_identity);
        window_update(&window);
    }

    destroy_window(&window);

    return 0;
}
