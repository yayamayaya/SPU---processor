#include "processor.h"

int arithmetics(Stack <data_t> *stackName, const int lowerMask);
int caseJMP(Stack <data_t> *stackName, bytecode_t *bytecode, const int cmd, const int address);
char *getCmd(const bytecode_t *bytecode, const signed int skipNum);
int ipInc(bytecode_t *bytecode, const int incNum);
int doCommand(processor_t *processor, bytecode_t *bytecode);
char *getCmdAndInc(bytecode_t *bytecode, const int numSize);
void takeOneArgument(processor_t *SPU, bytecode_t *bytecode, uint8_t argFlags, data_t **argument);

FILE *log_file = NULL;

int main(const int argc, const char** argv)
{
    assert(argc >= 2);
    _OPEN_LOG("logs/spu.log");

    LOG("> reading a bytecode file\n");
    bytecode_t bytecode = {0};
    file_read(&bytecode.bytecode, &bytecode.bytecodeSize, argv[1]);

    processor_t processor = {};
    LOG("> initialising stacks\n");
    processor.stk.stackCtor(10, "logs/dataStack.log");
    processor.returnPtrs.stackCtor(5, "logs/returnStack.log");

    LOG("> starting command sequence\n");
    int errorNum = 0;
    for (; bytecode.ip < bytecode.bytecodeSize;)
    {
        errorNum = doCommand(&processor, &bytecode);
        if (errorNum)
            break;
    }

    LOG("> commands sequence completed, ending programm\n");
    processor.stk.stackDtor();
    processor.returnPtrs.stackDtor();
    free(bytecode.bytecode);
    _CLOSE_LOG();

    if (errorNum == 1)
        return 0;
    
    LOG("> programm ended unsuccesfully, ending with error %d", errorNum);
    return errorNum;
}

int arithmetics(Stack <data_t> *stackName, const int cmd)
{
    assert(stackName != NULL);
    data_t temp1 = 0;
    data_t temp2 = 0;

    LOG("> doing arithmetical operation:\n");
    if (stackName->stackPop(&temp2) || stackName->stackPop(&temp1))
    {
        LOG("[error]>>> stack pop gave an error\n");
        printf("[error]>>> stack pop unsuccessfull doing arithm. operation\n");
        return STACK_POP_FAIL;
    }
    LOG("> arithm operation with %.2lf and %.2lf\n", temp1, temp2);
       
    int error = 0;
    switch (cmd)
    {
    case ADD:   CALC(+);
    case SUB:   CALC(-);
    case MULT:  CALC(*);
    case DIV:   CALC(/);

    default:
        LOG("[error]>>> fatal error in arithm operation\n");
        error = FATAL_ERR;
        break;
    }

    return error;
}

int caseJMP(Stack <data_t> *stackName, bytecode_t *bytecode, const int cmd, const int address)
{
    assert(stackName != NULL);
    assert(bytecode != NULL);
    data_t temp1 = 0;
    data_t temp2 = 0;

    if (stackName->stackPop(&temp2) || stackName->stackPop(&temp1))
    {
        LOG("[error]>>> stack pop gave an error\n");
        printf("[error]>>> stack pop unsuccessfull doing comparison operation\n");
        return STACK_POP_FAIL;
    }

    LOG("> comparison operation between %.2lf and %.2lf\n", temp1, temp2);
    /*if (stackName->stackPush(temp1))
        {ERRLOGRET(RPUSH_CMD_FAIL, msgZeroArgs, "RPUSH_CMD_FAIL", "", bytecode->ip, "[error]");}*/

    switch(cmd)
    {
    case JB:    CMP(<);
    case JBE:   CMP(<=);
    case JA:    CMP(>);
    case JAE:   CMP(>=);
    case JE:    CMP(==);
    case JNE:   CMP(!=);

    default:
        LOG("[error]>>> fatal error in cmp operation\n");
        return FATAL_ERR;
    }

    return 0;
}

void takeOneArgument(processor_t *SPU, bytecode_t *bytecode, uint8_t argFlags, data_t **argument)
{   
    assert(SPU);
    assert(bytecode);
    assert(argument);

    LOG("> taking an argument:\n");
    if (argFlags & NUM_ARG && argFlags & REG_ARG)
    {

        data_t regHolder = SPU->Register[(int)*getCmdAndInc(bytecode, sizeof(uint8_t))];
        data_t cmdHolder = (data_t)*(int *)getCmdAndInc(bytecode, sizeof(int));

        **argument = regHolder + cmdHolder;
    }

    else if (argFlags & NUM_ARG)
        *argument = (data_t *)getCmdAndInc(bytecode, sizeof(data_t));

    else if (argFlags & REG_ARG)  
        *argument = (data_t *)&SPU->Register[(int)*getCmdAndInc(bytecode, sizeof(uint8_t))];

    else if (argFlags & ADR_ARG)
        *(int **)argument = (int *)getCmdAndInc(bytecode, sizeof(int));        

    else
        LOG("[error]>>> argument taking error\n");

    if (argFlags & RAM_ARG)
        *argument = (data_t *)&SPU->RAM[(int)**argument];
    
    LOG("> argument taken is: %lf\n", **argument);
    return;
}

int doCommand(processor_t *processor, bytecode_t *bytecode)
{
    assert(processor != NULL);
    assert(bytecode != NULL);    

    uint8_t cmd = *getCmdAndInc(bytecode, sizeof(uint8_t));

    static data_t valueHolder1 = 0;
    static data_t valueHolder2 = 0;

    data_t *argHolder1 = &valueHolder1;
    data_t *argHolder2 = &valueHolder2;
    
    int n = 0;
    int error = 0;

    if (cmd & 0x80)
    {
        uint8_t argFlags = *getCmdAndInc(bytecode, sizeof(uint8_t));
        if (cmd & 0x10)
        {
            takeOneArgument(processor, bytecode, argFlags >> 4, &argHolder1);
            takeOneArgument(processor, bytecode, argFlags, &argHolder2);
        }
        else
        {
            takeOneArgument(processor, bytecode, argFlags, &argHolder1);
        }
    }

    switch (cmd)
    {
    case PUSH_GRP:
        if (processor->stk.stackPush(*argHolder1))
        {
            error = PUSH_CMD_FAIL;
            LOG("[error]>>> push command fail\n");
        }
        break;
    
    case ADD:
    case SUB:
    case MULT:
    case DIV:
        error = arithmetics(&processor->stk, cmd);
        break;

    case SQRT:
        if (processor->stk.stackPop(argHolder2))
        {
            error = RPOP_CMD_FAIL;
            LOG("[error]>>> pop command fail\n");
            return error;
        }

        if (processor->stk.stackPush(sqrt(*argHolder2)))
        {
            error = PUSH_CMD_FAIL;
            LOG("[error]>>> push command fail\n");
        }
        break;
        
    case POP_GRP:
        if (processor->stk.stackPop(argHolder1))
        {
            error = RPOP_CMD_FAIL;
            LOG("[error]>>> ram pop command fail\n");
            return error;
        }
        break;
    
    case IN:
        printf("Enter the number:\n");
        if(!scanf(DATA_SPEC, argHolder1, &n))
        {
            error = IN_CMD__PUSH_FAIL;
            LOG("[error]>>> in command fail\n");
            return error;
        }

        if(processor->stk.stackPush(valueHolder1))
        {
            error = PUSH_CMD_FAIL;
            LOG("[error]>>> push command fail\n");
        }
        break;

    case MOV_GRP:
        *argHolder1 = *argHolder2;
        break;

    case OUT:
    {
        data_t popData = 0;

        printf("\n>>> DATA DUMPING:\n");
        while (processor->stk.stackPop(&popData) != STK_EMPTY)
            data_print(stderr, popData);
        printf(">>> DUMP ENDED.\n");
        break; 
    }
        
    case HALT:
        LOG("> programm halted\n");
        return PROGRAMM_ENDED;

    case CALL:
        if (processor->returnPtrs.stackPush(bytecode->ip))
        {
            LOG("[error]>>> log command push fail\n");
            return CALL_CMD__PUSH_FAIL;
        }
        bytecode->ip = *(int *)argHolder1;
        break;

    case JMP:
        bytecode->ip = *(int *)argHolder1;
        break;

    case JB:
    case JBE:
    case JA:
    case JAE:
    case JE:
    case JNE:
        error = caseJMP(&processor->stk, bytecode, cmd, *(int *)argHolder1);
        break;

    case RET:
        if (processor->returnPtrs.stackPop(&bytecode->ip)) 
        {
            LOG("[error]>>> return command fail\n");
            error = RET_CMD_FAIL;
        }
        break;


    default:
        fprintf(stderr, "ERROR_CMD = %#04x\n\n", cmd);
        LOG("[error]>>> fatal error\n");
        error = FATAL_ERR;
        break;
    }

    return error;
}

char *getCmdAndInc(bytecode_t *bytecode, const int numSize)
{
    assert(bytecode != NULL);
    assert(numSize > 0);

    char *Holder = getCmd(bytecode, 0);
    LOG("> got command: %#04x\n", (unsigned char)*Holder);
    ipInc(bytecode, numSize);
    return Holder;
}

char *getCmd(const bytecode_t *bytecode, const signed int skipNum)
{
    assert(bytecode != NULL);

    return bytecode->bytecode + (int)bytecode->ip + skipNum;
}

int ipInc(bytecode_t *bytecode, const int incNum)
{
    assert(bytecode != NULL);

    LOG("> current position is %d\n", bytecode->ip + incNum);
    return bytecode->ip += incNum;
}