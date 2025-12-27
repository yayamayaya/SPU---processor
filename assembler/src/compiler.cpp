#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "asm.h"
#include "labels.h"
#include "tokenization.h"
#include "argument_parsing.h"
#include "bytecode_funcs.h"
#include "graphics.h"
#include "compiler.h"
#include "log.h"

bool is_command(const token_t *token);

#define COMMAND_CASE(cmd_type, function)                \
    case cmd_type:                                      \
    {                                                   \
        _SECURE(function);                              \
                                                        \
        break;                                          \
    }

int Compiler(AsmArrays *arrays)
{
    assert(arrays);

    static int comp_num = 0;
    comp_num++;

    Stack<token_t> *tokens   = &arrays->tokens;
    Stack<label_t> *labels   = &arrays->labels;
    bytecode_t     *bytecode = &arrays->bytecode;

    LOG("starting compilation");
    for (size_t token_n = 0; token_n < tokens->stack_size(); token_n++)
    {
        LOG("token number: %d", token_n);
        token_t *token = tokens->at(token_n);

        if ((token->type & STRING))
        {
            int ret_code = determine_label(labels, token, (int)bytecode->ip);
            if (ret_code != LABEL_OK)
                return ret_code;

            continue;
        }

        if (!is_command(token))
            return WRONG_LINE;

        byte_t cmd = token->data.code;

        switch (cmd & ARG_TYPE_MASK)
        {

        COMMAND_CASE( NO_ARGUMENT_CMD, push(bytecode, &cmd, sizeof(byte_t))                 );

        COMMAND_CASE( ARGUMENT_CMD,    write_command_with_argument(arrays, &token_n)        );

        COMMAND_CASE( SPEC_ARG_CMD,    write_command_with_label(arrays, &token_n, comp_num) );

        COMMAND_CASE( GRAPHICAL_CMD,   write_command_with_graphics(arrays, &token_n)        );
        
        default:
        {
            LOG_ERR("Command wasn't determined, fatal error");
            printf("Compilation error at line %lu: fatal error, unknown command\n", token->line);

            return COMMAND_NOT_DET;
        }

        }
    }

    LOG("Compilation #%d finished", comp_num);
    return 0;
}

#undef COMMAND_CASE

bool is_command(const token_t *token)
{
    if (!(token->type & CMD))
    {
        LOG_ERR("token is not a string nor a command");
        printf("Compilation error at line %lu: line doesn't start with neither string nor command\n", token->line);

        return false;
    }

    return true;
}