#include <stdio.h>
#include <assert.h>
#include "SPU_logs.h"

#ifndef SPU_CMD_READ
#define SPU_CMD_READ

typedef struct
{
    unsigned char *bytecode;
    int bytecode_size;
    int ip;
} bytecode_t;

unsigned char *get_cmd_and_inc(bytecode_t *bytecode, const int inc_num);

unsigned char *get_cmd(const bytecode_t *bytecode, const signed int bytes_to_skip);

unsigned int ip_inc(bytecode_t *bytecode, const int inc_num);

#endif