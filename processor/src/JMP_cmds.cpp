#include "JMP_cmds.h"

int case_JMP(Stack <data_t> *stack_name, bytecode_t *bytecode, const int cmd, const int address)
{
    assert(stack_name);
    assert(bytecode);
    data_t first_num = 0;
    data_t second_num = 0;

    if (stack_name->pop(&second_num) || stack_name->pop(&first_num))
    {
        SPU_LOG("[error]>>> stack pop gave an error\n");
        return STK_POP_FAIL;
    }

    SPU_LOG("> comparison operation between");
    SPU_LOG(DATA_PRINT_SPEC, first_num);
    SPU_LOG("and\n");
    SPU_LOG(DATA_PRINT_SPEC, second_num);
    
    if (stack_name->push(first_num))
    {
        SPU_LOG("[error]>>> push command fail\n");
        return RPUSH_CMD_FAIL;
    }

    switch(cmd)
    {
    case JB:    CMP(<);
    case JBE:   CMP(<=);
    case JA:    CMP(>);
    case JAE:   CMP(>=);
    case JE:    CMP(==);
    case JNE:   CMP(!=);

    default:
        SPU_LOG("[error]>>> fatal error in cmp operation\n");
        return FATAL_ERR;
    }

    return 0;
}