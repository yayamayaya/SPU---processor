#include "ALU.h"

int arithmetics(Stack <data_t> *stack_name, const char cmd)
{
    assert(stack_name);
    data_t first_num    = 0;
    data_t second_num   = 0;

    SPU_LOG("> doing arithmetical operation:\n");
    if (stack_name->pop(&second_num) || stack_name->pop(&first_num))
    {
        SPU_LOG("[error]>>> stack pop gave an error\n");
        return STACK_POP_FAIL;
    }
    SPU_LOG("> arithm. operation with ");
    SPU_LOG(DATA_PRINT_SPEC, first_num);
    SPU_LOG("and\n");
    SPU_LOG(DATA_PRINT_SPEC, second_num);
       
    int error = 0;
    switch (cmd)
    {
    case ADD:       CALC(+);
    case SUB:       CALC(-);
    case MULT:      CALC(*);
    case DIV:       CALC(/);
    case LOG_E:     CALC(==);
    case LOG_NE:    CALC(!=);
    case LOG_A:     CALC(>);
    case LOG_AE:    CALC(>=);
    case LOG_B:     CALC(<);
    case LOG_BE:    CALC(<=);

    case POW:     
        if (stack_name->push(pow(first_num, second_num)))                
        {                                                                               
            SPU_LOG("[error]>>> error while pushing a data in stack in arithm op\n");       
            stack_name->print(IN_CONSOLE);                                          
            error = ARITHM_CMD_FAIL;                                                    
        }                                                                               
        break;

    default:
        SPU_LOG("[error]>>> fatal error in arithm operation\n");
        error = FATAL_ERR;
        break;
    }

    return error;
}