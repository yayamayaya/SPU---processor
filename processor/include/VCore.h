#include "asm.h"
#include "stack.h"

#ifndef VCORE_H
#define VCORE_H

#include "virtual_machine.h"

typedef unsigned int ip_t;

#define REGISTER_NUMBER 4

typedef struct VCore_struct 
{
    VirtualMachine *bus;
    byte_t *bytecode; //как будто идеальное место для стека
    //ip??
    size_t bytecode_size;
    Stack<data_t> *stack;
    Stack<ip_t> *return_ips;
    data_t registers[REGISTER_NUMBER];
} VCore;

typedef struct VCore_starter_struct
{
    VCore *core;
    const char *bytecode_filename;
} VCore_starter_st;

#define _RETURN_UPON_ERROR(statement, msg, ret_val, ...)\
    do {                                                \
        if (statement)                                  \
        {                                               \
            LOG_ERR(msg);                               \
            __VA_ARGS__;                                \
            return ret_val;                             \
        }                                               \
    } while (0)

#define DEFAULT_BYTECODE_SIZE 256
#define DEFAULT_STACK_SIZE    16


int run_vcore(void *starter);

int create_vcore(
    VCore *vcore, size_t bytecode_size = DEFAULT_BYTECODE_SIZE, size_t stack_capacity = DEFAULT_STACK_SIZE);

void destroy_vcore(VCore *vcore);

#endif