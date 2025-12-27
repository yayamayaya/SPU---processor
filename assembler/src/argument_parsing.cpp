#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log.h"
#include "commands.h"
#include "argument_parsing.h"
#include "bytecode_funcs.h"
#include "tokenization.h"

byte_t take_second_argument_if_mov(AsmArrays *arrays, size_t *token_number, const byte_t cmd_group);

bool is_valid_argument(const byte_t cmd_group, const byte_t arg_mask, const line_t line);

byte_t parse_argument_expression(AsmArrays *arrays, size_t *token_number);

bool check_syntax_token(const token_t *token, const byte_t value);

byte_t write_argument(bytecode_t *bytecode, const token_t *token);


int write_command_with_argument(AsmArrays *arrays, size_t *token_number)
{
    assert(arrays);
    assert(token_number);

    bytecode_t *bytecode = &arrays->bytecode;
    Stack<token_t> *tokens = &arrays->tokens;
    token_t *token = tokens->at(*token_number);
    assert(token);
    assert(token->type & CMD);

    byte_t cmd_group     = token->data.code;
    (*token_number)++;

    size_t cmd_pos = bytecode_reserve_space(bytecode, sizeof(uint16_t));
    if (cmd_pos == (size_t)-1)
        return STK_MEM_RLC_ERR;

    byte_t arg_mask1 = parse_argument_expression(arrays, token_number);
    if (arg_mask1 & 0xF0)
        return arg_mask1;

    if (!is_valid_argument(cmd_group, arg_mask1, token->line))
        return FIRST_ARG_IS_NUM_ERR;

    byte_t arg_mask2 = take_second_argument_if_mov(arrays, token_number, cmd_group);
    if (arg_mask2 & 0xF0)
        return arg_mask2;

    byte_t argument_number = arg_mask2 == 0 ? ONE_ARG : TWO_ARGS;

    uint16_t cmd = (uint16_t)((cmd_group) | ((argument_number | arg_mask2 << 3 | arg_mask1) << 8));
    bytecode_insert_reserved(bytecode, cmd_pos, &cmd, sizeof(uint16_t));

    return 0;
}

byte_t take_second_argument_if_mov(AsmArrays *arrays, size_t *token_number, const byte_t cmd_group)
{
    if (cmd_group != MOV_GRP)
        return 0;
    
    Stack<token_t> *tokens = &arrays->tokens;
    (*token_number)++;

    token_t *syntax_token = tokens->at(*token_number);
    if (!check_syntax_token(syntax_token, COMMA))
    {
        LOG_ERR("missing commma");
        printf("Compilation error at line %lu: missing comma in mov command\n", syntax_token->line);

        return MISSING_COMMA;
    }
    (*token_number)++;

    return parse_argument_expression(arrays, token_number);
}

bool is_valid_argument(const byte_t cmd_group, const byte_t arg_mask, const line_t line)
{
    if ((cmd_group == MOV_GRP || cmd_group == POP_GRP) 
        && arg_mask == NUMBER_TYPE)
    {
        LOG_ERR("Argument is number");
        printf("Compilation error at line %lu: you can't write number as an value reciever in command\n", line);

        return false;
    }

    return true;
}

#define CHECK_FOR_NULL_TOKEN(token)                                                                         \
    if (!token)                                                                                             \
    {                                                                                                       \
        printf("Compilation error: you really shouldn't put half-eaten arguments at the end of file\n");    \
        return NULL_TOKEN;                                                                                  \
    }

byte_t parse_argument_expression(AsmArrays *arrays, size_t *token_number)
{
    assert(arrays);
    assert(token_number);

    byte_t ram_mask = 0;
    byte_t arg_mask = 0;

    Stack<token_t> *tokens = &arrays->tokens;
    bytecode_t *bytecode = &arrays->bytecode;

    LOG("parsing argument expression");
    token_t *token = tokens->at(*token_number);
    CHECK_FOR_NULL_TOKEN(token);
    if (check_syntax_token(token, OP_SQ_BR))
    {
        LOG("'[' was found");
        (*token_number)++;

        ram_mask = RAM_TYPE;
    }

    token = tokens->at(*token_number);
    CHECK_FOR_NULL_TOKEN(token);
    arg_mask = write_argument(bytecode, token);
    if (arg_mask == ARG_PARS_ERR)
        return arg_mask;

    token = tokens->at(*token_number + 1);
    CHECK_FOR_NULL_TOKEN(token);
    if (!check_syntax_token(token, PLUS))
    {
        if (ram_mask && !check_syntax_token(token, CL_SQ_BR))
        {
            printf("Compilation error at line %lu: closing square bracket wasn't found, check your code\n", token->line);
            return CL_BR_NOT_FOUND;
        }

        if (ram_mask)
            (*token_number)++;

        return ram_mask | arg_mask;
    }

    (*token_number) += 2;

    byte_t number_mask = 0;

    token = tokens->at(*token_number);
    CHECK_FOR_NULL_TOKEN(token);
    number_mask = write_argument(bytecode, token);
    if (number_mask == ARG_PARS_ERR)
        return number_mask;
    (*token_number)++;

    if (number_mask != NUMBER_TYPE)
    {
        printf("Compilation error at line %lu: you can sum only with numbers, check your code\n", token->line);
        return ARG_NOT_NUM_ERR;
    }

    token = tokens->at(*token_number);
    CHECK_FOR_NULL_TOKEN(token);
    if (ram_mask && !check_syntax_token(token, CL_SQ_BR))
    {
        printf("Compilation error at line %lu: closing square bracket wasn't found, check your code\n", token->line);
        return CL_BR_NOT_FOUND;
    }

    return (ram_mask | arg_mask | number_mask);
}

#undef CHECK_FOR_NULL

bool check_syntax_token(const token_t *token, const byte_t value)
{
    assert(value);

    return (token->type == SYNTAX) && (token->data.code == value);
}

byte_t write_argument(bytecode_t *bytecode, const token_t *token)
{
    assert(bytecode);
    assert(token);

    if (token->type & NUMBER)
    {
        LOG("number was found");
        if (push(bytecode, &token->data.number, sizeof(data_t)))
            return ARG_PARS_ERR;

        return NUMBER_TYPE;
    }
    
    if (token->type & REGISTER)
    {
        LOG("register was found");
        if (push(bytecode, &token->data.code, sizeof(byte_t)))
            return ARG_PARS_ERR;

        return REGISTER_TYPE;
    }

    printf("Compilation error at line %lu: Couldn't detect an argument\n", token->line);
    return ARG_PARS_ERR;
}