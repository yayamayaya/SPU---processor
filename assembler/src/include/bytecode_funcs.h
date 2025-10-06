#include "bytecode_gen.h"

#ifndef BYTECODE_GEN_H
#define BYTECODE_GEN_H

#define _SECURE(call)           \
    do {                        \
        int ret_val = call;     \
        if (ret_val)            \
            return ret_val;     \
    } while (0)

int ctor(bytecode_t *bytecode, const size_t capacity);

void dtor(bytecode_t *bytecode);

int push(bytecode_t *bytecode, const void *data, const size_t data_size);

size_t bytecode_reserve_space(bytecode_t *bytecode, const size_t data_size);

void bytecode_insert_reserved(bytecode_t *bytecode, size_t pos, const void *data, const size_t data_size);

#endif