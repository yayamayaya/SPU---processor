#include "asm_arrays.h"

int allocate_arrays_memory(asm_arrays *arrays)
{
    arrays->bytecode = {NULL, INITIAL_BYTECODE_SIZE, 0};
    arrays->bytecode.bytecode_holder = (uint8_t *)calloc(arrays->bytecode.bytecode_holder_size, sizeof(uint8_t));
    if (!arrays->bytecode.bytecode_holder)
    {
        printf("[error]>>> Couldn't allocate memory for bytecode\n");
        free(arrays->tokens.tokens);
        return BYTECODE_MEM_ALC_ERR;
    }

    open_gen_log();

    int error = arrays->labels.Ctor(10, "logs/label_stack.log");
    return error;
}

void free_arrays_memory(asm_arrays *arrays)
{
    free(arrays->tokens.tokens);
    free(arrays->bytecode.bytecode_holder);
    arrays->labels.Dtor();

    close_gen_log();
}