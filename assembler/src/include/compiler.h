#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "asm.h"
#include "labels.h"
#include "tokenization.h"
#include "bytecode_gen.h"

#ifndef ASM_COMPILER
#define ASM_COMPILER

enum ASM_COMP_ERRS
{
    SAME_LABEL  = 1231,
    NOT_LABEL   = 1413,
    WRONG_LINE  = 1512,
    JMP_NOT_STRING,
    LABEL_NOT_FOUND,
};

int Compiler(token_arr *tokens, bytecode_t *bytecode, Stack <label_t> *labels);

#endif