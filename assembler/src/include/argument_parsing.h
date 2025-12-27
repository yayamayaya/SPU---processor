#include <stdint.h>

#include "tokenization.h"
#include "asm_arrays.h"

#ifndef ARGUMENT_PARSING_H
#define ARGUMENT_PARSING_H

enum ARGUMENT_PARSING_ERRS
{
    FIRST_ARG_IS_NUM_ERR        = 0x502,
    BYTECODE_MEM_ALC_ERR        = 0x503,
    
    // Special return codes for parse_argument_expression function
    CL_BR_NOT_FOUND             = 0x10,
    ARG_NOT_NUM_ERR             = 0x20,
    NULL_TOKEN                  = 0x30,
    MISSING_COMMA               = 0x40,
    ARG_PARS_ERR                = 0x50,
};

int write_command_with_argument(AsmArrays *arrays, size_t *token_number);

#endif
