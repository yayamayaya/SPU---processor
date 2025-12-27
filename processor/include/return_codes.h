#ifndef RETURN_CODE_H
#define RETURN_CODE_H

enum SPU_RETURN_CODE 
{
    OK                      = 0,
    INVALID_RAM_SIZE        = 0x01,
    MEMORY_ALC_ERR          = 0x02,
    INVALID_VCORE           = 0x04,
    BYTECODE_LOAD_ERR       = 0x08,
};

#endif