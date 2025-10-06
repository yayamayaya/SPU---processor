#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "string.h"
#include "stack.h"
#include "log.h"
#include "labels.h"

int labeldet(Stack <label_t> *labels, const char *label_name)
{
    assert(labels);
    assert(label_name);

    for (int pos = 0; pos < labels->get_stack_size(); pos++)
    {
        assert(labels->get_data_on_pos(pos).name);

        if (!strcmp(labels->get_data_on_pos(pos).name, label_name))
            return labels->get_data_on_pos(pos).ip;
    }

    return -1;
}

int label_insert(Stack <label_t> *labels, const char *label_name, const int ip)
{
    assert(labels);
    assert(label_name);

    return labels->push((label_t){label_name, ip});
}

void label_table_print(Stack <label_t> *labels)
{
    assert(labels);

    printf("------------------------------------------------------------\n");
    printf(">> Table size: %d\n", labels->get_stack_size());
    //LOG(">>Number of labels: %d\n", labels->labelCounter);
    for (int i = 0; i < labels->get_stack_size(); i++)
        printf(">> [%d] Label Name: %s, label address: %d\n", i, labels->get_data_on_pos(i).name, labels->get_data_on_pos(i).ip);
    printf("Table ended.\n");
    printf("------------------------------------------------------------\n");
}
