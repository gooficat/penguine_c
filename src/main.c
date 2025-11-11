#include <stdio.h>
#include "window/window.h"

int main() {
    window_s window = create_window(640, 360, "penguine");

    printf("Hello, World!\n");
    
    show_window(&window);

    while (!window_should_close(&window)) {
        window_update(&window);
    }
    return 0;
}