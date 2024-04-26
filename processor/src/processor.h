#ifndef PROCESSOR
#define PROCESSOR

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>
#include "../../assembler/src/include/asm.h"
#include "../../../common/file_reading/src/file_reading.h"
#include "../../../stack/src/stack.h"
#include "../../../common/logging/log.h"

#define SIZE_OF_RAM 256

#define CALC(arg)                                                                   \
    if (stackName->stackPush(temp1 arg temp2))                                      \
    {                                                                               \
        printf("[error]>>> stk push fail\n");                                       \
        LOG("[error]>>> error while pushing a data in stack in arithm op\n");       \
        stackName->stackPrint(IN_CONSOLE);                                          \
        error = ARITHM_CMD_FAIL;                                                    \
    }                                                                               \
    break

#define CMP(arg)                    \
    if (temp1 arg temp2)            \
        bytecode->ip = address;     \
    break

typedef struct
{
    class Stack <data_t> stk;
    class Stack <int> returnPtrs;
    data_t RAM[SIZE_OF_RAM];
    data_t Register[REGISTERS_NUM];
} processor_t;

typedef struct
{
    char *bytecode;
    int bytecodeSize;
    int ip;
} bytecode_t;

enum DEBUG_CODE
{
    FATAL_ERROR = 1001,
    PROGRAMM_ENDED = 1,
    ARITHM_GRP_ERR = 100,
    PUSH_CMD_FAIL = 10,
    RPUSH_CMD_FAIL,
    RAMPUSH_CMD_FAIL,
    RPOP_CMD_FAIL,
    RAMPOP_CMD_FAIL,
    IN_CMD__SCAN_ERR,
    IN_CMD__PUSH_FAIL,
    MOV_CMD_FAIL,
    CALL_CMD__PUSH_FAIL,
    RET_CMD_FAIL,
    ARITHM_CMD_FAIL,
    ARG_TAKE_FAIL,
    STACK_POP_FAIL,
};

enum BITMASKS
{
    MOST_SIGN_MASK = 0xF0,
    LESS_SIGN_MASK = 0x0F,
};

#endif