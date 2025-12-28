#include "utilities/file.h"
#include <stdio.h>

char* file_to_string(const char* path, const char* mode)
{
    FILE* f;
    fopen_s(&f, path, mode);
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    char* content = calloc(size + 1, sizeof(char));
    fread(content, sizeof(char), size, f);
    content[size] = '\0';

    return content;
}
