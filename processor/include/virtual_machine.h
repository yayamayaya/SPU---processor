#include "asm.h"
#include "VCore.h"

#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

typedef struct VirtualMachine_struct 
{
    byte_t *RAM; //File will be here?
    VCore *virtual_core;
} VirtualMachine;

int run_virtual_machine();

int create_virtual_machine(VirtualMachine *spu);

void destroy_virtual_machine(VirtualMachine *spu);

#endif