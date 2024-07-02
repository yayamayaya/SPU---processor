#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "asm.h"
#include "tokenization.h"
#include "log.h"

#ifndef BYTECODEGEN
#define BYTECODEGEN

#define INITIAL_BYTECODE_SIZE 150

typedef struct
{
    uint8_t *bytecode_holder;
    int bytecode_holder_size;
    int ip;
} bytecode_t;

enum BYTECODEGEN_ERRS
{
    CL_BR_NOT_FOUND         = 5125,
    ARG_PARS_ERR            = 1561,
    FIRST_ARG_IS_NUM_ERR    = 5124,
    BYTECODE_MEM_ALC_ERR    = 1111,
    BC_FILE_OPEN_ERR        = 1001,
    FILEWRITE_ERR,
    MISSING_COMMA
};

void open_gen_log();

void close_gen_log();

void cmd_in_arr(bytecode_t *bytecode, const byte_t cmd_num);

void num_in_arr(bytecode_t *bytecode, void *number_ptr, const int number_size);

int arg_in_arr(bytecode_t *bytecode, int *token_number, token_arr *arg_tokens);

int arg_cmd(bytecode_t *bytecode, int *token_number, token_arr *tokens);

int bytecode_to_file(const char *file_name, bytecode_t *bytecode);

#endif