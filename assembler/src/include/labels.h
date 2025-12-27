#include "stack.h"
#include "tokenization.h"

#ifndef ASM_LABELS
#define ASM_LABELS

typedef struct 
{
    const char *name;
    int ip;
} label_t;

enum LABEL_WORK
{
    LABEL_OK        = 0x200,
    SAME_LABEL      = 0x201,
    NOT_LABEL       = 0x202,
    JMP_NOT_STRING  = 0x203,
    LABEL_NOT_FOUND = 0x204,
};

void label_table_print(Stack <label_t> *labels);

int determine_label(Stack <label_t> *labels, token_t *token, const int ip);

#include "asm_arrays.h"

int write_command_with_label(AsmArrays *arrays, size_t *token_number, const int comp_num);

#endif