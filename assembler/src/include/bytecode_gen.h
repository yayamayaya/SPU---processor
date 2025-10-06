#include <stdint.h>

#include "tokenization.h"

#ifndef BYTECODEGEN
#define BYTECODEGEN

#define INITIAL_BYTECODE_SIZE 150

typedef struct
{
    uint8_t *bytecode_holder;
    size_t capacity;
    size_t ip;
} bytecode_t;

enum BYTECODEGEN_ERRS
{
    CL_BR_NOT_FOUND             = 5125,
    ARG_PARS_ERR                = 1561,
    FIRST_ARG_IS_NUM_ERR        = 5124,
    BYTECODE_MEM_ALC_ERR        = 1111,
    BC_FILE_OPEN_ERR            = 1001,
    FILEWRITE_ERR               = 1511,
    MISSING_COMMA               = 1516,
    NEGATIVE_BYTECODE_CAPACITY  = 7256
};

int arg_cmd(bytecode_t *bytecode, int *token_number, Stack<token_t> *tokens);

int bytecode_to_file(const char *file_name, bytecode_t *bytecode);

#endif
