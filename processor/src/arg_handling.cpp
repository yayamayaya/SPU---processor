#include "arg_handling.h"

int take_argument(processor_t *SPU, bytecode_t *bytecode, uint8_t arg_flags, data_t **argument)
{   
    assert(SPU);
    assert(bytecode);
    assert(argument);

    SPU_LOG("> taking an argument:\n");
    if (arg_flags & NUM_ARG && arg_flags & REG_ARG)
    {
        SPU_LOG("> taking registger with shift\n");

        data_t reg_holder       = SPU->reg[(int)*get_cmd_and_inc(bytecode, sizeof(uint8_t))];
        data_t address_holder   = (data_t)*(int *)get_cmd_and_inc(bytecode, sizeof(int));

        **argument = reg_holder + address_holder;
    }

    else if (arg_flags & NUM_ARG)
        *argument = (data_t *)get_cmd_and_inc(bytecode, sizeof(data_t));

    else if (arg_flags & REG_ARG)  
        *argument = &SPU->reg[(int)*get_cmd_and_inc(bytecode, sizeof(uint8_t))];

    else if (arg_flags & ADR_ARG)
        *(int **)argument = (int *)get_cmd_and_inc(bytecode, sizeof(int));        

    else
    {
        SPU_LOG("[error]>>> argument taking error\n");
        
        return ARG_TAKE_ERROR;
    }

    if (arg_flags & RAM_ARG)
        *argument = &SPU->RAM[(int)**argument];
    
    //SPU_LOG("> argument taken is: %lf\n", **argument);
    return 0;
}