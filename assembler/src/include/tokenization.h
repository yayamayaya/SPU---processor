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
    CMD         = 1,
    SYNTAX      = 2,
    REGISTER    = 3,
    NUMBER      = 4,
    STRING      = 5,
};

enum TOKEN_ERRORS
{
    REG_NOT_EXIST       = 551,
    TOKEN_MEM_ALC_ERR   = 552,
    TOKEN_MEM_RLC_ERR   = 553,
    FILEREAD_ERR        = 554,
};

#define _POS_SHIFT(arg)                                                                             \
    pos += arg;                                                                                     \
    LOG("position shifted, current position is %d, current char is '%c'", pos, buff[pos])

#define _SKIP_SPACES()                                                                              \
    LOG("skipping spaces");                                                                         \
    while (isspace(buff[pos]))                                                                      \
        (pos)++;                                                                                    \
                                                                                                    \
    LOG("spaces skipped, current position is: %d, current char is: '%c'", pos, buff[pos])


int tokenize(Stack<token_t> *tokens);

#endif
