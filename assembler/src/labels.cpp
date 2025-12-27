#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "string.h"
#include "stack.h"
#include "log.h"
#include "labels.h"

int label_find(Stack <label_t> *labels, const char *label_name);

int label_insert(Stack <label_t> *labels, const char *label_name, const int ip);


int write_command_with_label(AsmArrays *arrays, size_t *token_number, const int comp_num)
{
    assert(arrays);
    assert(token_number);

    bytecode_t *bytecode   = &arrays->bytecode;
    Stack<label_t> *labels = &arrays->labels;
    Stack<token_t> *tokens = &arrays->tokens;
    token_t *token = tokens->at(*token_number);
    assert(token);
    assert(token->type & CMD);

    _SECURE(push(bytecode, &token->data.code, sizeof(byte_t)));

    (*token_number)++;
    token_t label_token = *tokens->at(*token_number);

    if (label_token.type != STRING)
    {
        LOG_ERR("jmp argument is not a label");
        printf("Compilation error at line %lu: jmp or call command don't have the right argument for it\n"
                "Note: argument must be a label\n", label_token.line);

        return JMP_NOT_STRING;
    }

    int label_IP = label_find(labels, label_token.data.string);
    if (comp_num == 2 && label_IP == -1)
    {
        LOG_ERR("label def. not found");
        printf("Compilation error at line %lu: label \"%s\" definition not found\n",
                label_token.line, label_token.data.string);

        return LABEL_NOT_FOUND;
    }

    _SECURE(push(bytecode, &label_IP, sizeof(int)));
    return 0;
}

int label_find(Stack <label_t> *labels, const char *label_name)
{
    assert(labels);
    assert(label_name);

    for (size_t pos = 0; pos < labels->stack_size(); pos++)
        if (!strcmp(labels->at(pos)->name, label_name))
            return labels->at(pos)->ip;

    return -1;
}

int label_insert(Stack <label_t> *labels, const char *label_name, const int ip)
{
    assert(labels);
    assert(label_name);

    labels->push((label_t){label_name, ip});
    return (int)labels->get_stack_error();
}

void label_table_print(Stack <label_t> *labels)
{
    assert(labels);

    printf("---------------------Labels dump-------------------------\n");
    printf("Table size: %lu\n", labels->stack_size());
    for (size_t i = 0; i < labels->stack_size(); i++)
        printf("[%lu]Label name: %s, label address: %d\n", i, labels->at(i)->name, labels->at(i)->ip);
    printf("---------------------------------------------------------\n");
}

int determine_label(Stack <label_t> *labels, token_t *token, const int ip)
{
    assert(labels);
    assert(token);
    assert(token->type & STRING);

    char *str_token = token->data.string;
    char *colon_pointer = strchr(str_token, ':');

    if (colon_pointer)
    {
        LOG("colon was found in a string token, checking for an existing label");
        *colon_pointer = '\0';

        int ret_val = label_find(labels, str_token);
        if (ret_val != -1)
        {
            LOG_ERR("a label with the same name '%s' is already existing on ip: '%d'", token->data.string, ret_val);
            printf("Compilation error at line %lu: programm cannot have 2 labels with the same name\n", token->line);

            return SAME_LABEL;
        }

        int error = label_insert(labels, token->data.string, ip);
        error = error == 0 ? LABEL_OK : error;

        return error;
    }

    int label_IP = label_find(labels, str_token);
    if (label_IP != -1)
        return LABEL_OK;

    LOG_ERR("a string token '%s' is not a label", str_token);
    printf("Compilation error at line %lu: '%s' is not a label nor a command, check your syntax\n", token->line, str_token);

    return NOT_LABEL;
}

