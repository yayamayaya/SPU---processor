#include "stack.h"

#ifndef ASM_LABELS
#define ASM_LABELS

typedef struct 
{
    const char *name;
    int ip;
} label_t;

enum LABEL_WORK
{
    LABEL_EXIST     = 134,
    LABEL_INSERT    = 135,
    SAME_LABEL  = 1231,
    NOT_LABEL   = 1413,
};

int labeldet(Stack <label_t> *labels, const char *label_name);

int label_insert(Stack <label_t> *labels, const char *label_name, const int ip);

void label_table_print(Stack <label_t> *labels);

#endif