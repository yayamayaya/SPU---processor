#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "string.h"
#include "stack.h"
#include "log.h"

#ifndef ASM_LABELS
#define ASM_LABELS

typedef struct 
{
    const char *name;
    int ip;
} label_t;

typedef struct
{
    label_t *label_arr;   
    int label_table_size;     
    int label_counter;
} label_table;

enum LABEL_WORK
{
    LABEL_EXIST     = 134,
    LABEL_INSERT    = 135,
};

int labeldet(Stack <label_t> *labels, const char *label_name);

int label_insert(Stack <label_t> *labels, const char *label_name, const int ip);

void label_table_print(Stack <label_t> *labels);

#endif