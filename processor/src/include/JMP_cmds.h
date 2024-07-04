#include <stdio.h>
#include <assert.h>
#include <SPU_logs.h>
#include "stack.h"
#include "cmd_read.h"
#include "asm.h"

#ifndef SPU_JMP_CMDS
#define SPU_JMP_CMDS

#define CMP(arg)                    \
    if (first_num arg second_num)   \
        bytecode->ip = address;     \
    break

enum SPU_JMP_CMDS_ERRS
{
    STK_POP_FAIL  = 7451,
    RPUSH_CMD_FAIL  = 7541
};

int case_JMP(Stack <data_t> *stack_name, bytecode_t *bytecode, const int cmd, const int address);

#endif