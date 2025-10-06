#include "asm_arrays.h"

#ifndef ASM_COMPILER
#define ASM_COMPILER

enum ASM_COMP_ERRS
{
    WRONG_LINE  = 1512,
    JMP_NOT_STRING,
    LABEL_NOT_FOUND,
};

int Compiler(AsmArrays *arrays);

#endif