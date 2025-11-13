#include "window/window.h"
#include <corecrt_search.h>
#include <libloaderapi.h>
#include <wingdi.h>
#include <winuser.h>

#ifdef UNICODE
#undef UNICODE
#endif


LRESULT CALLBACK window_procedure(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    switch (Msg) {
        case WM_DESTROY:
        case WM_CLOSE:
            PostMessageA(hwnd, WM_QUIT, wParam, lParam);
            return 0;
        default:
            break;
    }
    return DefWindowProcA(hwnd, Msg, wParam, lParam);
}

window_s create_window(uint16_t width, uint16_t height, const char *title) {
#ifdef _WIN32
    window_s window = {0};

    HINSTANCE hinstance = GetModuleHandleA(NULL);

    WNDCLASSA wc = {
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpszClassName = title,
        .lpfnWndProc = window_procedure,
        .hInstance = hinstance,
        .hIcon = LoadIconA(hinstance, IDI_WINLOGO),
        .hCursor = LoadCursorA(hinstance, IDC_ARROW),
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .lpszMenuName = NULL,
        .hbrBackground = NULL
    };
    RegisterClassA(&wc);

    window.hwnd = CreateWindowExA(
        0,//WS_EX_COMPOSITED,
        title,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL,
        NULL,
        hinstance,
        NULL
    );

    window.hdc = GetDC(window.hwnd);

    PIXELFORMATDESCRIPTOR pfd = {
        .nVersion = 1,
        .cColorBits = 24,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .nSize = sizeof(pfd),
        .cDepthBits = 24,
        .iLayerType = PFD_MAIN_PLANE
    };

    int pf = ChoosePixelFormat(window.hdc, &pfd);
    SetPixelFormat(window.hdc, pf, &pfd);

    DescribePixelFormat(window.hdc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

#endif
    window.should_close = 0;

    return window;
}

void window_make_context_current(window_s *window) {
    window->hrc = wglCreateContext(window->hdc);
    wglMakeCurrent(window->hdc, window->hrc);
}

void show_window(window_s *window) {
    ShowWindow(window->hwnd, SW_SHOW);
}

int window_should_close(window_s *window) {
    return window->should_close;
}


void window_update(window_s *window) {
#ifdef _WIN32
    MSG msg;
    while (PeekMessageA(&msg, window->hwnd, 0, 0, PM_REMOVE) > 0) {
        if (msg.message == WM_QUIT) {
            window->should_close = 1;
            CloseWindow(window->hwnd);
            // return;
        }
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    // window->should_close = 0;

    SwapBuffers(window->hdc);
#endif
}


void destroy_window(window_s *window) {
#ifdef _WIN32
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(window->hrc);
    window->hrc = NULL;
    ReleaseDC(window->hwnd, window->hdc);
    window->hdc = NULL;
    DestroyWindow(window->hwnd);
#endif
}