#include "commands.h"

cmd_t commands[COMMANDS_NUM] = {
{"push", PUSH_GRP, 4},
{"add", ADD, 3},
{"sub", SUB, 3},
{"mult", MULT, 4},
{"div", DIV, 3},
{"ret", RET, 3},
{"out_i", OUTI, 5},
{"out", OUT, 3},
{"halt", HALT, 4},
{"in", IN, 2},
{"pop", POP_GRP, 3},
{"jmp", JMP, 3},
{"jb", JB, 2},
{"jbe", JBE, 3},
{"ja", JA, 2},
{"jae", JAE, 3},
{"je", JE, 2},
{"jne", JNE, 3},
{"call", CALL, 4},
{"mov", MOV_GRP, 3},
{"sqrt", SQRT, 4},
{"pow", POW, 3},
{"log_e", LOG_E, 5},
{"log_ne", LOG_NE, 6},
{"log_a", LOG_A, 5},
{"log_ae", LOG_AE, 6},
{"log_b", LOG_B, 5},
{"log_be", LOG_BE, 6}
};

syntax_ch syntax_chars[SYNTAX_CH_NUMBER] =
{
    {'[', OP_SQ_BR},
    {']', CL_SQ_BR},
    {'+', PLUS},
    {',', COMMA}
};