#include "render/gl.h"

#ifdef _WIN32
#include <Windows.h>
#include <libloaderapi.h>
#include <wingdi.h>
#endif

void * load_gl_func(const char * name);

void load_gl() {
    glViewport = load_gl_func("glViewport");
    glClearColor = load_gl_func("glClearColor");
    glClear = load_gl_func("glClear");
}





#ifdef _WIN32
void * load_gl_func(const char * name) {
    void * p = (void *)wglGetProcAddress(name);
    if (p == 0
     || p == (void*)0x1
     || p == (void*)0x2
     || p == (void*)0x3
     || p == (void*)-1) {
        HMODULE gl_module = LoadLibraryA("opengl32.dll");
        p = (void *)GetProcAddress(gl_module, name);
    }
    return p;
}
#endif