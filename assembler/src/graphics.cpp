#include "graphics.h"
#include "file_reading.h"
#include "log.h"

int write_command_with_graphics(AsmArrays *arrays, size_t *token_number)
{
    assert(arrays);
    assert(token_number);

    bytecode_t *bytecode   = &arrays->bytecode;
    Stack<token_t> *tokens = &arrays->tokens;
    token_t *token = tokens->at(*token_number);
    assert(token);
    assert(token->type & CMD);


    (*token_number)++;
    token_t *filepath = tokens->at(*token_number);
    if (!(filepath->type & STRING))
    {
        printf("Compilation error at line %lu: invalid 'load' command usage.\n"
               "Please check that file name doesn't match any commands\n", token->line);

        return INVALID_FILE_NAME;
    }

    char *filebuf    = NULL;
    size_t file_size = 0;

    _SECURE(file_read(&filebuf, &file_size, filepath->data.string));
    if (!file_size)
    {
        printf("'load' command at line %lu: file is empty, command will be skipped\n", token->line);
        return 0;
    }

    _SECURE(push(bytecode, &token->data.code, sizeof(byte_t)));
    _SECURE(push(bytecode, &file_size, sizeof(size_t)));
    _SECURE(push(bytecode, filebuf, file_size * sizeof(char)));

    free(filebuf);

    return 0;
}