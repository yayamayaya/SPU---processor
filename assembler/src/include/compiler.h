#include "asm_arrays.h"

#ifndef ASM_COMPILER
#define ASM_COMPILER

enum ASM_COMP_ERRS
{
    WRONG_LINE      = 0x300,
    COMMAND_NOT_DET = 0x303,
};

int Compiler(AsmArrays *arrays);

#endif