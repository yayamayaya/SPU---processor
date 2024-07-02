#include <stdio.h>
#include <stdlib.h>
#include "tokenization.h"
#include "labels.h"
#include "bytecode_gen.h"
#include "log.h"

#ifndef ASM_ARRAYS
#define ASM_ARRAYS

typedef struct
{
    bytecode_t bytecode;
    Stack <label_t> labels;
    token_arr tokens;
} asm_arrays;

int allocate_arrays_memory(asm_arrays *arrays);

void free_arrays_memory(asm_arrays *arrays);

#endif