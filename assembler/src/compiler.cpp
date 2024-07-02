#include "compiler.h"

_INIT_LOG();

int determine_label(Stack <label_t> *labels, token_t *token, const int ip);

int Compiler(token_arr *tokens, bytecode_t *bytecode, Stack <label_t> *labels) 
{
    assert(tokens);
    assert(bytecode);
    assert(labels);

    _OPEN_N_LOG("logs/compilation_%d.log");

    static int comp_num = 0;
    comp_num++;

    LOG("> starting compilation\n");
    for (int token_n = 0; token_n < tokens->token_num; token_n++)
    {
        LOG("> token number: %d\n", token_n);

        if (tokens->tokens[token_n].type == STRING)
        {
            int ret_code = determine_label(labels, tokens->tokens + token_n, bytecode->ip);
            if      (ret_code == LABEL_INSERT || ret_code == LABEL_EXIST)
                continue;
            else if (ret_code == NOT_LABEL || ret_code == SAME_LABEL)
            {
                _CLOSE_LOG();
                return ret_code;
            }
        }

        if (tokens->tokens[token_n].type != CMD)
        {
            LOG("[c_error]>>> token in a new line is not a string nor command\n");
            printf("> Compilation error: new line doesn't contain neither string nor command\n");

            _CLOSE_LOG();
            return WRONG_LINE;
        }
        
        if (bytecode->ip >= bytecode->bytecode_holder_size * 0.75)
        {
            LOG("> bytecode holder array is too small, scailing it by 2 times:\n");
            bytecode->bytecode_holder_size *= 2;
            uint8_t *temp_bt_holder = (uint8_t *)realloc(bytecode->bytecode_holder, bytecode->bytecode_holder_size * sizeof(uint8_t));
            if (!temp_bt_holder)
            {
                LOG("[error]>>> bytecode array memory reallocation error\n");
                return BYTECODE_MEM_ALC_ERR;
            }
            
            bytecode->bytecode_holder = temp_bt_holder;
        }

        
        LOG("> label detection finished, determining a command:\n");
        unsigned char cmd = tokens->tokens[token_n].data.code;
        
        switch (cmd)
        {
        case PUSH_GRP:
        case POP_GRP:
        case MOV_GRP:
        {
            LOG("> argument command %#04X found\n", cmd);
            int err_code = arg_cmd(bytecode, &token_n, tokens);
            if (err_code)
            {
                _CLOSE_LOG();   
                return err_code;
            }
            break;
        }

        case CALL:
        case JMP:
        case JB:
        case JBE:
        case JA:
        case JAE:
        case JE:
        case JNE:
        {
            LOG("> jmp command %#04X found\n", cmd);
            uint16_t jmp_cmd = cmd | (ADR_ARG << 8);
            num_in_arr(bytecode, &jmp_cmd, sizeof(uint16_t));
            token_n++;
            if (tokens->tokens[token_n].type != STRING)
            {
                LOG("[c_error]>>> jmp argument is not a label\n");
                printf("> Compilation error: jmp or call command don't have the right argument for it (argument must be a label)\n");
                _CLOSE_LOG();
                return JMP_NOT_STRING;
            }
            
            int label_IP = labeldet(labels, tokens->tokens[token_n].data.string);
            if (comp_num == 2 && !label_IP)
            {
                LOG("[c_error]>>> label def. not found\n");
                printf("> Compilation error: label \"%s\" definition not found\n", tokens->tokens[token_n].data.string);
                _CLOSE_LOG();
                return LABEL_NOT_FOUND;
            }
            
            num_in_arr(bytecode, &label_IP, sizeof(int));
            break;
        }

        case ADD:
        case SUB:
        case MULT:
        case DIV:
        case SQRT:
        case IN:
        case OUT:
        case HALT:
        case LOG_A:
        case LOG_AE:
        case LOG_E:
        case LOG_NE:
        case LOG_B:
        case LOG_BE:
        case POW:
        case RET:
        case OUTI:
            LOG("> no argument command detected\n");
            cmd_in_arr(bytecode, cmd);

            break;
        
        default:
            LOG("[error]>>> command wasn't determined, fatal error\n");
            printf("> Compilation error: fatal error\n");
            _CLOSE_LOG();
            return FATAL_ERR;
        } 
    }

    LOG("> Compilation #%d finisged\n", comp_num);
    _CLOSE_LOG();
    return 0;
}


int determine_label(Stack <label_t> *labels, token_t *token, const int ip)
{
    assert(labels);
    assert(token);
    assert(token->type == STRING);

    char *str_token = token->data.string;
    char *colon_pointer = strchr(str_token, ':');

    if (colon_pointer) 
    {
        LOG("colon was found in a string token, checking for an existing label\n");
        *colon_pointer = '\0';
        if (labeldet(labels, str_token))
        {
            LOG("[c_error]>>> a label with the same name is already existing\n");
            printf("> Compilation error: programm cannot have 2 labels with the same name.\n");

            return SAME_LABEL;
        }
        
        label_insert(labels, token->data.string, ip);
        return LABEL_INSERT;
    }

    int label_IP = labeldet(labels, str_token);
    if (label_IP)
    {
        LOG("> a string token is a determined label, continuing..\n");
        return LABEL_EXIST;
    }

    LOG("[c_error]>>> a string token %s is not a label\n", str_token);
    printf("> Compilation error: \"%s\" is not a label nor a command, check your syntax\n", str_token);

    return NOT_LABEL;
}