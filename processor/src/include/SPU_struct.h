#include <stdio.h>
#include "asm.h"
#include "stack.h"

#ifndef SPU_STRUCT
#define SPU_STRUCT

#define SIZE_OF_RAM     256
#define REGISTERS_NUM   4

typedef struct
{
    class Stack <data_t> stk;
    class Stack <int> return_ptrs;
    data_t RAM[SIZE_OF_RAM];
    data_t reg[REGISTERS_NUM];
} processor_t;

#endif