#include <stdint.h>

#ifndef BYTECODE_GEN_H
#define BYTECODE_GEN_H

#define INITIAL_BYTECODE_SIZE 128

#define _SECURE(call)           \
    do {                        \
        int ret_val = call;     \
        if (ret_val)            \
            return ret_val;     \
    } while (0)


typedef struct
{
    uint8_t *bytecode_holder;
    size_t capacity;
    size_t ip;
} bytecode_t;

enum BYTECODE_STACK_ERRORS
{
    STK_MEM_ALC_ERR             = 0x400,
    STK_MEM_RLC_ERR             = 0x401,
    BC_FILE_OPEN_ERR            = 0x402,
    FILEWRITE_ERR               = 0x403,
    NEGATIVE_BYTECODE_CAPACITY  = 0x404,
};

int ctor(bytecode_t *bytecode, const size_t capacity);

void dtor(bytecode_t *bytecode);

int push(bytecode_t *bytecode, const void *data, const size_t data_size);

size_t bytecode_reserve_space(bytecode_t *bytecode, const size_t data_size);

void bytecode_insert_reserved(bytecode_t *bytecode, size_t pos, const void *data, const size_t data_size);

int bytecode_to_file(const char *file_name, bytecode_t *bytecode);

#endif