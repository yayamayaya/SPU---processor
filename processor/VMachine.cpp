#include "virtual_machine.h"
#include "cmdline_options.h"
#include "return_codes.h"
#include "log.h"

int run_virtual_machine()
{
    VirtualMachine spu = {};
    create_virtual_machine(&spu);

    // There will be virtual core selector and code runner with memory management
    // code will be stored in ram

    destroy_virtual_machine(&spu);
    return OK;
}

int create_virtual_machine(VirtualMachine *spu)
{
    assert(spu);

    size_t ram_size = get_ram_amount();
    if (!ram_size)
    {
        LOG_ERR("invalid ram_size");
        return INVALID_RAM_SIZE;
    }

    spu->RAM = (byte_t *)calloc(ram_size, sizeof(byte_t));
    if (!spu->RAM)
    {
        LOG_ERR("RAM memory allocation error")
        return MEMORY_ALC_ERR;
    }

    spu->virtual_core = (VCore *)calloc(1, sizeof(VCore));
    if (!spu->virtual_core)
    {
        LOG_ERR("VCORE memory allocation error");
        free(spu->RAM);
        return MEMORY_ALC_ERR;
    }

    spu->virtual_core->bus = spu;
    //create_vcore(spu->virtual_core);

    return OK;
}

void destroy_virtual_machine(VirtualMachine *spu)
{
    free(spu->RAM);

    //destroy_vcore(spu->virtual_core);
    free(spu->virtual_core);

    spu->RAM = NULL;
    spu->virtual_core = NULL;
}