#include "asm.h"
#include "stack.h"

#ifndef ASM_TOKEN
#define ASM_TOKEN

#define TOKEN_NAME_MAX_LENGTH 50

typedef union
{
    data_t number;
    unsigned char code;
    char string[TOKEN_NAME_MAX_LENGTH];
} token_data;

typedef unsigned long int line_t;

typedef struct
{
    line_t line;
    token_data data;
    unsigned char type;
} token_t;

enum TOKEN_TYPES
{
    CMD         = 0x01,
    SYNTAX      = 0x02,
    REGISTER    = 0x04,
    NUMBER      = 0x08,
    STRING      = 0x10,
};

enum TOKEN_ERRORS
{
    TOKENIZATION_OK     = 0x100,
    TOKENIZATION_BAD    = 0x101,
    FILEREAD_ERR        = 0x102,
};

int tokenize(Stack<token_t> *tokens);

#endif
