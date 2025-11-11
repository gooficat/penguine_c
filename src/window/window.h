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
#endif
    uint16_t width;
    uint16_t height;
} window_s;

window_s create_window(uint16_t width, uint16_t height, const char *title);

void show_window(window_s *window);

int window_should_close(window_s *window);

void window_update(window_s *window);