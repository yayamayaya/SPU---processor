#include "switch.h"

int do_command(processor_t *processor, bytecode_t *bytecode)
{
    assert(processor != NULL);
    assert(bytecode != NULL);    

    uint8_t cmd = *get_cmd_and_inc(bytecode, sizeof(uint8_t));

    static data_t arg_holder1 = 0;
    static data_t arg_holder2 = 0;

    data_t *arg_ptr1 = &arg_holder1;
    data_t *arg_ptr2 = &arg_holder2;
    
    //int n = 0;
    int error = 0;

    if (cmd & 0x80)
    {
        uint8_t arg_flags = *get_cmd_and_inc(bytecode, sizeof(uint8_t));
        take_argument(processor, bytecode, arg_flags >> 4, &arg_ptr1);

        if (cmd & 0x10)
            take_argument(processor, bytecode, arg_flags, &arg_ptr2);
    }

    switch (cmd)
    {
    case PUSH_GRP:
        if (processor->stk.push(*arg_ptr1))
        {
            error = PUSH_CMD_FAIL;
            SPU_LOG("[error]>>> push command fail\n");
        }
        break;
    
    case ADD:
    case SUB:
    case MULT:
    case DIV:
    case LOG_E:
    case LOG_NE:
    case LOG_A:
    case LOG_AE:
    case LOG_B:
    case LOG_BE:
    case POW:
        error = arithmetics(&processor->stk, cmd);
        break;

    case SQRT:
        if (processor->stk.pop(arg_ptr2))
        {
            error = RPOP_CMD_FAIL;
            SPU_LOG("[error]>>> pop command fail\n");
            return error;
        }

        if (processor->stk.push(sqrt(*arg_ptr2)))
        {
            error = PUSH_CMD_FAIL;
            SPU_LOG("[error]>>> push command fail\n");
        }
        break;
        
    case POP_GRP:
        if (processor->stk.pop(arg_ptr1))
        {
            error = RPOP_CMD_FAIL;
            SPU_LOG("[error]>>> ram pop command fail\n");
            return error;
        }
        break;
    
    case IN:
        printf("Enter the number:\n");
        if (!scanf(DATA_SPEC, arg_ptr1))
        {
            error = IN_CMD__PUSH_FAIL;
            SPU_LOG("[error]>>> in command fail\n");
            return error;
        }

        if(processor->stk.push(*arg_ptr1))
        {
            error = PUSH_CMD_FAIL;
            SPU_LOG("[error]>>> push command fail\n");
        }
        break;

    case MOV_GRP:
        *arg_ptr1 = *arg_ptr2;
        break;

    case OUTI:
    {
        data_t popped_data = 0;
        printf("--> ");
        processor->stk.pop(&popped_data);
        printf(DATA_PRINT_SPEC, popped_data);
        break; 
    }

    case OUT:
    {
        data_t popped_data = 0;

        printf("\n>>> DATA DUMPING:\n");
        while (processor->stk.pop(&popped_data) != STK_EMPTY)
            printf(DATA_PRINT_SPEC, popped_data);
        printf(">>> DUMP ENDED.\n");
        break; 
    }
        
    case HALT:
        SPU_LOG("> programm halted\n");
        return PROGRAMM_ENDED;

    case CALL:
        if (processor->return_ptrs.push(bytecode->ip))
        {
            SPU_LOG("[error]>>> log command push fail\n");
            return CALL_CMD__PUSH_FAIL;
        }
        bytecode->ip = *(int *)arg_ptr1;
        break;

    case JMP:
        bytecode->ip = *(int *)arg_ptr1;
        break;

    case JB:
    case JBE:
    case JA:
    case JAE:
    case JE:
    case JNE:
        error = case_JMP(&processor->stk, bytecode, cmd, *(int *)arg_ptr1);
        break;

    case RET:
        if (processor->return_ptrs.pop(&bytecode->ip)) 
        {
            SPU_LOG("[error]>>> return command fail\n");
            error = RET_CMD_FAIL;
        }
        break;


    default:
        printf("ERROR CMD = %#04x\n\n", cmd);
        SPU_LOG("[error]>>> fatal error\n");
        error = FATAL_ERR;
        break;
    }

    return error;
}