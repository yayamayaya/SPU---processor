#include <stdio.h>
#include <assert.h>
#include "cmd_read.h"
#include "JMP_cmds.h"
#include "SPU_struct.h"
#include "SPU_logs.h"
#include "arg_handling.h"
#include "ALU.h"

#ifndef SPU_SWITCH
#define SPU_SWITCH

enum DEBUG_CODE
{
    PROGRAMM_ENDED = 1,
    ARITHM_GRP_ERR = 100,
    PUSH_CMD_FAIL = 10,
    RAMPUSH_CMD_FAIL,
    RPOP_CMD_FAIL,
    RAMPOP_CMD_FAIL,
    IN_CMD__SCAN_ERR,
    IN_CMD__PUSH_FAIL,
    MOV_CMD_FAIL,
    CALL_CMD__PUSH_FAIL,
    RET_CMD_FAIL,
    //ARITHM_CMD_FAIL,
    ARG_TAKE_FAIL,
};

int do_command(processor_t *processor, bytecode_t *bytecode);

#endif