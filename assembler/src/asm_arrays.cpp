#include "asm_arrays.h"
#include "bytecode_funcs.h"

int allocate_arrays_memory(AsmArrays *arrays)
{
    assert(arrays);

    arrays->tokens.enable_stack_log("tokens_stack.log");
    arrays->labels.enable_stack_log("label_stack.log");

    return ctor(&arrays->bytecode, 50);
}

void free_arrays_memory(AsmArrays *arrays) {
    dtor(&arrays->bytecode);
}
