#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "asm.h"
#include "labels.h"
#include "tokenization.h"
#include "bytecode_gen.h"
#include "bytecode_funcs.h"
#include "compiler.h"
#include "log.h"

int determine_label(Stack <label_t> *labels, token_t *token, const int ip);

int Compiler(AsmArrays *arrays)
{
    assert(arrays);

    static int comp_num = 0;
    comp_num++;

    Stack<token_t> *tokens   = &arrays->tokens;
    Stack<label_t> *labels   = &arrays->labels;
    bytecode_t     *bytecode = &arrays->bytecode;

    LOG("starting compilation");
    for (int token_n = 0; token_n < tokens->get_stack_size(); token_n++)
    {
        LOG("token number: %d", token_n);

        token_t *token = tokens->get_data_ptr() + token_n;

        if (token->type == STRING)
        {
            int ret_code = determine_label(labels, token, (int)bytecode->ip);

            if      (ret_code == LABEL_INSERT || ret_code == LABEL_EXIST)
                continue;
            else if (ret_code == NOT_LABEL    || ret_code == SAME_LABEL )
                return ret_code;
        }

        if (token->type != CMD)
        {
            LOG_ERR("token is not a string nor a command");
            printf("Compilation error at line %lu: line doesn't contain neither string nor command\n", token->line);

            return WRONG_LINE;
        }

        LOG("label detection finished, determining a command...");
        unsigned char cmd = token->data.code;

        switch (cmd)
        {
        case PUSH_GRP:
        case POP_GRP:
        case MOV_GRP:
        {
            LOG("argument command %#04X found", cmd);
            _SECURE(arg_cmd(bytecode, &token_n, tokens));

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
            LOG("jmp command %#04X found", cmd);

            uint16_t jmp_cmd = cmd | (ADR_ARG << 12);
            _SECURE(push(bytecode, &jmp_cmd, sizeof(uint16_t)));
            
            token_n++;
            token_t label_token = tokens->get_data_on_pos(token_n);
            if (label_token.type != STRING)
            {
                LOG_ERR("jmp argument is not a label");
                printf("Compilation error at line %lu: jmp or call command don't have the right argument for it\n"
                       "Note: argument must be a label\n", label_token.line);

                return JMP_NOT_STRING;
            }

            int label_IP = labeldet(labels, label_token.data.string);
            if (comp_num == 2 && label_IP == -1)
            {
                LOG_ERR("label def. not found");
                printf("Compilation error at line %lu: label \"%s\" definition not found\n", label_token.line, label_token.data.string);

                label_table_print(labels);

                return LABEL_NOT_FOUND;
            }

            _SECURE(push(bytecode, &label_IP, sizeof(int)));
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
            LOG("no argument command detected");
            _SECURE(push(bytecode, &cmd, sizeof(unsigned char)));

            break;

        default:
            LOG_ERR("Command wasn't determined, fatal error");
            printf("Compilation error at line %lu: fatal error, unknown command\n", token->line);

            return FATAL_ERR;
        }
    }

    LOG("Compilation #%d finished", comp_num);
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
        LOG("colon was found in a string token, checking for an existing label");
        *colon_pointer = '\0';

        int ret_val = labeldet(labels, str_token);
        if (ret_val != -1)
        {
            LOG_ERR("a label with the same name '%s' is already existing on ip: '%d'", token->data.string, ret_val);
            printf("Compilation error at line %lu: programm cannot have 2 labels with the same name\n", token->line);

            return SAME_LABEL;
        }

        label_insert(labels, token->data.string, ip);
        return LABEL_INSERT;
    }

    int label_IP = labeldet(labels, str_token);
    if (label_IP != -1)
    {
        LOG("a string token is a determined label, continuing");
        return LABEL_EXIST;
    }

    LOG_ERR("a string token '%s' is not a label", str_token);
    printf("Compilation error at line %lu: '%s' is not a label nor a command, check your syntax\n", token->line, str_token);

    return NOT_LABEL;
}
