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

    glViewport(0, 0, 640, 360);

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    while (!window_should_close(&window)) {
        window_update(&window);

        glClear(GL_COLOR_BUFFER_BIT);
    }

    destroy_window(&window);

    exit(EXIT_SUCCESS);
}