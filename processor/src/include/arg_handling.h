#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "cmd_read.h"
#include "SPU_struct.h"

#ifndef SPU_ARG_HANDLING
#define SPU_ARG_HANDLING

enum ARG_HANDLE_ERRS
{
    ARG_TAKE_ERROR = 1441
};

int take_argument(processor_t *SPU, bytecode_t *bytecode, uint8_t arg_flags, data_t **argument);

#endif
