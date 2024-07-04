#include "cmd_read.h"

unsigned char *get_cmd_and_inc(bytecode_t *bytecode, const int inc_num)
{
    assert(bytecode);
    assert(inc_num > 0);

    unsigned char *holder = get_cmd(bytecode, 0);
    SPU_LOG("> got command: %#04x\n", (unsigned char)*holder);
    ip_inc(bytecode, inc_num);
    return holder;
}

unsigned char *get_cmd(const bytecode_t *bytecode, const signed int bytes_to_skip)
{
    assert(bytecode);

    return bytecode->bytecode + (int)bytecode->ip + bytes_to_skip;
}

unsigned int ip_inc(bytecode_t *bytecode, const int inc_num)
{
    assert(bytecode);

    SPU_LOG("> current position is %d\n", bytecode->ip + inc_num);
    return bytecode->ip += inc_num;
}