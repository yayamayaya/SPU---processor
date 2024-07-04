#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "stack.h"
#include "SPU_logs.h"
#include "asm.h"

#ifndef SPU_ALU
#define SPU_ALU

#define CALC(arg)                                                                   \
    if (stack_name->push(first_num arg second_num))                                 \
    {                                                                               \
        SPU_LOG("[error]>>> error while pushing a data in stack in arithm op\n");       \
        stack_name->print(IN_CONSOLE);                                              \
        error = ARITHM_CMD_FAIL;                                                    \
    }                                                                               \
    break

enum ALU_ERRS
{
    STACK_POP_FAIL      = 1237,
    ARITHM_CMD_FAIL,
};

int arithmetics(Stack <data_t> *stack_name, const char cmd);

#endif