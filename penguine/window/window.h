#pragma once
#include <stdint.h>

#ifdef UNICODE
#undef UNICODE
#endif

#ifdef _WIN32

#include <Windows.h>

#endif


typedef struct window {
#ifdef _WIN32
    HWND hwnd;
    HDC hdc;
    HGLRC hrc;
#endif
    uint16_t width;
    uint16_t height;
    int8_t should_close;
} window_s;

window_s create_window(uint16_t width, uint16_t height, const char *title);

void window_make_context_current(window_s *window);

void show_window(window_s *window);

int window_should_close(window_s *window);

void window_update(window_s *window);

void destroy_window(window_s *window);