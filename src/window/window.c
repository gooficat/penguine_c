#include "window.h"
#include <corecrt_search.h>
#include <libloaderapi.h>
#include <winuser.h>


LRESULT CALLBACK window_procedure(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProcA(hwnd, Msg, wParam, lParam);
}

window_s create_window(uint16_t width, uint16_t height, const char *title) {
#ifdef _WIN32
    window_s window = {0};

    HINSTANCE hinstance = GetModuleHandleA(NULL);

    WNDCLASSA wc;
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = title;
    wc.lpfnWndProc = window_procedure;
    wc.hInstance = hinstance;
    wc.hIcon = LoadIconA(hinstance, IDI_WINLOGO);
    wc.hCursor = LoadCursorA(hinstance, IDC_ARROW);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpszMenuName = NULL;
    wc.hbrBackground = NULL;
    RegisterClassA(&wc);

    window.hwnd = CreateWindowExA(
        0,
        title,
        title,
        WS_OVERLAPPEDWINDOW | WS_EX_COMPOSITED,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL,
        NULL,
        hinstance,
        NULL
    );
#endif

    return window;
}

void show_window(window_s *window) {
    ShowWindow(window->hwnd, SW_SHOW);
}

int window_should_close(window_s *window) {
    window->height = window->height; //shush the debugger
    return 0;
}


void window_update(window_s *window) {
#ifdef _WIN32
    static MSG msg;
    if (PeekMessageA(&msg, window->hwnd, 0, 0, PM_REMOVE) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
#endif
}