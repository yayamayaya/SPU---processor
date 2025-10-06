#include <string.h>

#include "bytecode_gen.h"
#include "bytecode_funcs.h"
#include "log.h"

int bytecode_realloc(bytecode_t *bytecode, const size_t new_capacity);

int ctor(bytecode_t *bytecode, const size_t capacity) 
{
    assert(bytecode);

    if (!(capacity > 0))
    {
        LOG_ERR("bytecode capacity 0 or less");
        return NEGATIVE_BYTECODE_CAPACITY;
    }

    uint8_t *array = (uint8_t *)calloc(capacity, sizeof(uint8_t));
    if (!array)
    {
        LOG_ERR("bytecode array memory alloction error");
        return STK_MEM_ALC_ERR;
    }

    bytecode->bytecode_holder = array;
    bytecode->capacity = capacity;
    bytecode->ip = 0;

    return 0;
}

void dtor(bytecode_t *bytecode)
{
    assert(bytecode);

    free(bytecode->bytecode_holder);

    bytecode->bytecode_holder = NULL;
    bytecode->capacity = 0;
    bytecode->ip = 0;
}

int push(bytecode_t *bytecode, const void *data, const size_t data_size)
{
    assert(bytecode);
    assert(bytecode->bytecode_holder);
    assert(data);

    size_t pos = bytecode_reserve_space(bytecode, data_size);
    if (pos == (size_t)-1)
        return STK_MEM_RLC_ERR;

    bytecode_insert_reserved(bytecode, pos, data, data_size);

    return 0;
}

size_t bytecode_reserve_space(bytecode_t *bytecode, const size_t data_size)
{
    assert(bytecode);

    if ((bytecode->ip + data_size >= bytecode->capacity) 
        && bytecode_realloc(bytecode, bytecode->capacity * 2))
    {
        LOG_ERR("bytecode array extension error");
        return (size_t)-1;
    }

    size_t pos = bytecode->ip;
    bytecode->ip += data_size;

    return pos;
}

void bytecode_insert_reserved(bytecode_t *bytecode, size_t pos, const void *data, const size_t data_size)
{
    assert(bytecode);
    assert(data);

    memcpy(bytecode->bytecode_holder + pos, data, data_size);
}

int bytecode_realloc(bytecode_t *bytecode, const size_t new_capacity)
{
    assert(bytecode);

    if (!(new_capacity > 0))
    {
        LOG_ERR("bytecode capacity 0 or less");
        return STK_MEM_RLC_ERR;
    }

    uint8_t *temp_arr = (uint8_t *)realloc(bytecode->bytecode_holder, new_capacity * sizeof(uint8_t));
    if (!temp_arr)
    {
        LOG_ERR("bytecode realloc error");
        return STK_MEM_RLC_ERR;
    }

    bytecode->bytecode_holder = temp_arr;
    bytecode->capacity = new_capacity;

    return 0;
}