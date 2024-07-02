#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "commands.h"
#include "log.h"
#include "file_reading.h"

#ifndef ASM_TOKEN
#define ASM_TOKEN

typedef union
{
    data_t number;
    unsigned char code;
    char string[50];
} token_data;

typedef struct
{
    token_data data;
    unsigned char type;
} token_t;

typedef struct
{
    token_t *tokens;
    int token_num;
} token_arr;

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
    TOKEN_MEM_ALC_ERR, 
    TOKEN_MEM_RLC_ERR,
    FILEREAD_ERR,
};

#define _POS_SHIFT(arg)                                                                             \
    (*pos) += arg;                                                                                  \
    LOG("> position shifted, current position is %d, current char is %c\n", *pos, buff[*pos])       

#define _SKIP_SPACES()                                                                              \
    LOG("> skipping spaces\n");                                                                     \
    while (isspace(buff[*pos]))                                                                     \
        (*pos)++;                                                                                   \
                                                                                                    \
    LOG("> spaces skipped, current position is: %d, current char is: %c\n", *pos, buff[*pos])


enum MACHINE_CODE cmdDet(const cmd *commands, const int commandsNumber, const char *token);

int tokenize(token_arr *tokens, const char *file_name);

#endif
