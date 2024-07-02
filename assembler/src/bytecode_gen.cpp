#include "bytecode_gen.h"

_INIT_LOG();

int argument_parsing(bytecode_t *bytecode, int *token_number, token_arr *tokens);

void open_gen_log()
{
    _OPEN_LOG("logs/codegen.log");
}

void close_gen_log()
{
    _CLOSE_LOG();
}

void cmd_in_arr(bytecode_t *bytecode, const byte_t cmd_num)
{
    assert(bytecode);

    bytecode->bytecode_holder[bytecode->ip] = cmd_num;
    bytecode->ip++;

    LOG("> /command %d was pushed in bytecode/\n", cmd_num);
}

void num_in_arr(bytecode_t *bytecode, void *number_ptr, const int number_size)
{
    assert(bytecode);
    assert(number_ptr);

    memcpy(bytecode->bytecode_holder + bytecode->ip, number_ptr, number_size);
    bytecode->ip += number_size;

    LOG("> /number was pushed in bytecode/\n");
}

int arg_in_arr(bytecode_t *bytecode, int *token_number, token_arr *arg_tokens)
{
    assert(arg_tokens);
    assert(token_number);
    assert(bytecode);

    int ram_mask         = 0;
    int arg_mask         = 0;

    token_t first_token = arg_tokens->tokens[*token_number];

    LOG("/parsing arguments, checking for square brackets:\n");

    if (first_token.type == SYNTAX && first_token.data.code == OP_SQ_BR)
    {
        LOG("> opening bracket was found, ram mask will be applied\n");
        (*token_number)++;
        ram_mask = RAM_ARG;
    }

    arg_mask = argument_parsing(bytecode, token_number, arg_tokens);
    if (arg_mask == ARG_PARS_ERR)
        return ARG_PARS_ERR;

    if (ram_mask)
    {
        (*token_number)++;
        if (arg_tokens->tokens[*token_number].type != SYNTAX || arg_tokens->tokens[*token_number].data.code != CL_SQ_BR)
        {
            LOG("[c_error]>>> closing bracket wasn't found/\n");
            printf("> Compilation error: closing square bracket wasn't found, check your code\n");

            return CL_BR_NOT_FOUND;
        }
    }

    LOG("> arguments were parsed/\n");
    return ram_mask | arg_mask;   
}

int argument_parsing(bytecode_t *bytecode, int *token_number, token_arr *tokens)
{
    assert(bytecode);
    assert(token_number);
    assert(tokens);

    token_t first_arg = tokens->tokens[*token_number];

    if (first_arg.type == NUMBER)
    {
        LOG("> number was found\n");
        num_in_arr(bytecode, &first_arg.data.number, sizeof(data_t));
        return NUM_ARG;
    }

    if (first_arg.type == REGISTER)
    {
        LOG("> register was found\n");
        cmd_in_arr(bytecode, first_arg.data.code);

        if (tokens->tokens[*token_number + 1].type == SYNTAX && tokens->tokens[*token_number + 1].data.code == PLUS)
        {
            (*token_number)++;
            LOG("> addition to register was found\n");
            (*token_number)++;
            if (tokens->tokens[*token_number].type == NUMBER)
            {
                LOG("> number to add was found\n");
                num_in_arr(bytecode, &tokens->tokens[*token_number].data.number, sizeof(data_t));
                
                return REGNUM_ARG;
            }
            else
            {
                LOG("[c_error]>>> number to add wasn't found/\n");
                printf("> Compilation error: number for reegister addition wasn't found, check your syntax\n Note: you can ONLY add numbers to registers\n");

                return ARG_PARS_ERR;
            }
        }
        return REG_ARG;
    }
    
    LOG("[c_error]>>> invalid argument type/\n");
    printf("> Compilation error: invalid argument for a command\n");
    return ARG_PARS_ERR;  
} 

//Парсинг команд с аргументами
int arg_cmd(bytecode_t *bytecode, int *token_number, token_arr *tokens)
{
    assert(bytecode);
    assert(token_number);
    assert(tokens);
    assert(tokens->tokens[*token_number].type == CMD);

    int arg_mask1 = 0;
    int arg_mask2 = 0;

    unsigned char cmd_group = tokens->tokens[*token_number].data.code;
    (*token_number)++;

    uint16_t *cmd_ptr = (uint16_t *)(bytecode->bytecode_holder + bytecode->ip);
    bytecode->ip += sizeof(uint16_t);

    arg_mask1 = arg_in_arr(bytecode, token_number, tokens);
    if (arg_mask1 == CL_BR_NOT_FOUND || arg_mask1 == ARG_PARS_ERR)
    {
        return arg_mask1;
    }
    
    if (cmd_group & 0x10)
    {
        (*token_number)++;
        if (tokens->tokens[*token_number].type != SYNTAX || tokens->tokens[*token_number].data.code != COMMA)
        {
            LOG("[c_error]>>> missing commma\n");
            printf("> Compilation error: missing comma in mov command\n");

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
        
    *cmd_ptr = (cmd_group) | ((arg_mask1 << 4 | arg_mask2) << 8);

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
        LOG("[error]>>> couldn't open bytecode file\n");
        return BC_FILE_OPEN_ERR;
    }
    
    if ((int)fwrite(bytecode->bytecode_holder, sizeof(char), bytecode->ip, bc_file) != bytecode->ip)
    {
        LOG("[error]>>> bytecode file write error\n");
        err_code = FILEWRITE_ERR;        
    }

    fclose(bc_file);
    return err_code;
}