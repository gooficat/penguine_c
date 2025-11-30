#pragma once

#include <stdio.h>

#define safe_realloc(mem, size) mem = realloc(mem, size); if (!mem){\
    printf("Reallocation failure");\
    exit(EXIT_FAILURE);\
}