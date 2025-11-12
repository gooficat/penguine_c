#pragma once

#include <stdint.h>


typedef float GLFloat;
typedef uint32_t GLBitfield;
typedef uint32_t GLuint;
typedef uint32_t GLenum;
typedef char GLchar;
typedef uint32_t GLsizei;
typedef int32_t GLint;

void load_gl();

#define GL_COLOR_BUFFER_BIT 0x4000

void (*glViewport)(int x, int y, int width, int height);

void (*glClearColor)(GLFloat r, GLFloat g, GLFloat b, GLFloat a);
void (*glClear)(GLBitfield mask);

GLuint (*glCreateShader)(GLenum type);
GLuint (*glShaderSource)(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
GLuint (*glCompileShader)(GLuint shader);

GLuint (*glDeleteShader)(GLuint shader);

GLuint (*glCreateProgram)();
GLuint (*glAttachShader)(GLuint shader, GLuint program);
GLuint (*glLinkProgram)(GLuint program);
void (*glUseProgram)(GLuint program);

void (*glGenBuffers)(GLuint count, GLuint *buffers);
void (*glGenVertexArrays)(GLuint count, GLuint *arrays);

void (*glDeleteBuffers)(GLuint count, GLuint *buffers);
void (*glDeleteVertexArrays)(GLuint count, GLuint *arrays);

void (*glBindBuffer)(GLenum target, GLuint buffer);
void (*glBindVertexArray)(	GLuint array);

