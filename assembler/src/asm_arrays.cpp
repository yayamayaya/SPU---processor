#include "asm_arrays.h"
#include "bytecode_funcs.h"

int allocate_arrays_memory(AsmArrays *arrays)
{
    assert(arrays);

    int error = ctor(&arrays->bytecode, 50);
    if (error)
        return error;

    error = arrays->tokens.Ctor(30, "logs/tokens_stack.log");
    if (error)
        return error;

    error = arrays->labels.Ctor(10, "logs/label_stack.log");
    return error;
}

void free_arrays_memory(AsmArrays *arrays) {
    dtor(&arrays->bytecode);
    arrays->tokens.Dtor();
    arrays->labels.Dtor();
}
