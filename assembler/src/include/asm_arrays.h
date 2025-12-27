#include "tokenization.h"
#include "labels.h"
#include "bytecode_funcs.h"
#include "stack.h"

#ifndef ASM_ARRAYS
#define ASM_ARRAYS

typedef struct
{
    bytecode_t      bytecode;
    Stack <label_t> labels;
    Stack <token_t> tokens;
} AsmArrays;

int allocate_arrays_memory(AsmArrays *arrays);

void free_arrays_memory(AsmArrays *arrays);

#endif