#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "asm_arrays.h"

enum GRAPHICS_ERRORS
{
    INVALID_FILE_NAME = 0x800,
};

int write_command_with_graphics(AsmArrays *arrays, size_t *token_number);

#endif