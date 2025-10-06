#include "asm.h"

#ifndef ASM_CMDS
#define ASM_CMDS

#define COMMANDS_NUM 28
#define REGISTERS_NUM 4

#define SYNTAX_CH_NUMBER 4

typedef struct
{
    const char *cmd_name;
    enum MACHINE_CODE cmd_num;
    unsigned char cmd_len;
} cmd_t;

enum SYNTAX_CH
{
    OP_SQ_BR    = 1,
    CL_SQ_BR    = 2,
    PLUS        = 3,
    COMMA       = 4,

    NO_SYNT     = 15
};

typedef struct
{
    const char syntax_char;
    enum SYNTAX_CH char_code;
} syntax_ch;

extern cmd_t commands[COMMANDS_NUM];
extern syntax_ch syntax_chars[SYNTAX_CH_NUMBER];

#endif