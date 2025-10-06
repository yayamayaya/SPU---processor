#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log.h"
#include "commands.h"
#include "bytecode_funcs.h"

int argument_parsing(bytecode_t *bytecode, int *token_number, Stack<token_t> *tokens);

int arg_in_arr(bytecode_t *bytecode, int *token_number, Stack<token_t> *tokens);

int arg_in_arr(bytecode_t *bytecode, int *token_number, Stack<token_t> *tokens)
{
    assert(token_number);
    assert(bytecode);
    assert(tokens);

    int ram_mask         = 0;
    int arg_mask         = 0;

    token_t first_token = tokens->get_data_on_pos(*token_number);

    LOG("parsing arguments, checking for square brackets:");

    if (first_token.type == SYNTAX && first_token.data.code == OP_SQ_BR)
    {
        LOG("opening bracket was found, ram mask will be applied");
        (*token_number)++;
        ram_mask = RAM_ARG;
    }

    arg_mask = argument_parsing(bytecode, token_number, tokens);
    if (arg_mask == ARG_PARS_ERR || arg_mask == STK_MEM_RLC_ERR)
        return ARG_PARS_ERR;

    if (ram_mask)
    {
        (*token_number)++;
        token_t bracket_token = tokens->get_data_on_pos(*token_number);
        if (bracket_token.type != SYNTAX || bracket_token.data.code != CL_SQ_BR)
        {
            LOG_ERR("closing bracket wasn't found");
            printf("Compilation error at line %lu: closing square bracket wasn't found, check your code\n", bracket_token.line);

            return CL_BR_NOT_FOUND;
        }
    }

    LOG("arguments were parsed");
    return ram_mask | arg_mask;
}

int argument_parsing(bytecode_t *bytecode, int *token_number, Stack<token_t> *tokens)
{
    assert(bytecode);
    assert(token_number);
    assert(tokens);

    token_t first_arg = tokens->get_data_on_pos(*token_number);

    if (first_arg.type == NUMBER)
    {
        LOG("number was found");
        _SECURE(push(bytecode, &first_arg.data.number, sizeof(data_t)));
        return NUM_ARG;
    }

    if (first_arg.type == REGISTER)
    {
        LOG("register was found");
        _SECURE(push(bytecode, &first_arg.data.code, sizeof(unsigned char)));

        token_t plus_token = tokens->get_data_on_pos(*token_number + 1);
        if (plus_token.type == SYNTAX && plus_token.data.code == PLUS)
        {
            LOG("addition to register was found");
            (*token_number) += 2;

            token_t addition_number = tokens->get_data_on_pos(*token_number);
            if (addition_number.type != NUMBER)
            {
                LOG_ERR("number to add wasn't found");
                printf("Compilation error at line %lu: number for register addition wasn't found, check your syntax\n"
                    "Note: you can ONLY add numbers to registers\n", addition_number.line);
                    
                    return ARG_PARS_ERR;
            }

            LOG("number to add was found");
            
            //check_for_int
            int number = (int)addition_number.data.number;
            _SECURE(push(bytecode, &number, sizeof(int)));

            return REGNUM_ARG;
        }
        return REG_ARG;
    }

    LOG_ERR("invalid argument type");
    printf("Compilation error at line %lu: invalid argument for a command\n", first_arg.line);
    return ARG_PARS_ERR;
}

// Парсинг команд с аргументами
int arg_cmd(bytecode_t *bytecode, int *token_number, Stack<token_t> *tokens)
{
    assert(bytecode);
    assert(token_number);
    assert(tokens);

    token_t cmd_token = tokens->get_data_on_pos(*token_number);
    assert(cmd_token.type == CMD);

    int arg_mask1 = 0;
    int arg_mask2 = 0;

    unsigned char cmd_group = cmd_token.data.code;
    (*token_number)++;

    size_t cmd_pos = bytecode_reserve_space(bytecode, sizeof(uint16_t));
    if (cmd_pos == (size_t)-1)
        return STK_MEM_RLC_ERR;

    arg_mask1 = arg_in_arr(bytecode, token_number, tokens);
    if (arg_mask1 == CL_BR_NOT_FOUND || arg_mask1 == ARG_PARS_ERR)
        return arg_mask1;

    // Checking if cmd is MOV
    if (cmd_group & 0x10)
    {
        (*token_number)++;

        token_t syntax_token = tokens->get_data_on_pos(*token_number);
        if (syntax_token.type != SYNTAX || syntax_token.data.code != COMMA)
        {
            LOG_ERR("missing commma");
            printf("Compilation error at line %lu: missing comma in mov command\n", syntax_token.line);

            return MISSING_COMMA;
        }
        (*token_number)++;

        if (arg_mask1 == NUM_ARG)
        {
            return FIRST_ARG_IS_NUM_ERR;
        }

        arg_mask2 = arg_in_arr(bytecode, token_number, tokens);
        if (arg_mask2 == CL_BR_NOT_FOUND || arg_mask2 == ARG_PARS_ERR)
            return arg_mask2;
    }

    uint16_t cmd = (uint16_t)((cmd_group) | ((arg_mask1 << 4 | arg_mask2) << 8));
    bytecode_insert_reserved(bytecode, cmd_pos, &cmd, sizeof(uint16_t));

    return 0;
}

int bytecode_to_file(const char *file_name, bytecode_t *bytecode)
{
    assert(file_name);
    assert(bytecode);

    int err_code = 0;

    FILE *bc_file = fopen(file_name, "wb");
    if (!bc_file)
    {
        LOG_ERR("couldn't open bytecode file");
        return BC_FILE_OPEN_ERR;
    }

    if (fwrite(bytecode->bytecode_holder, sizeof(char), bytecode->ip, bc_file) != bytecode->ip)
    {
        LOG_ERR("bytecode file write error");
        err_code = FILEWRITE_ERR;
    }

    fclose(bc_file);
    return err_code;
}
