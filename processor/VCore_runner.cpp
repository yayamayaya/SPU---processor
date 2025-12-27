#include "VCore.h"
#include "file_reading.h"
#include "return_codes.h"
#include "log.h"

int load_file_to_core(VCore *vcore, const char *filename);

bool is_valid_core(const VCore *vcore);

int run_vcore(void *starter)
{
    assert(starter);

    VCore_starter_st *start = (VCore_starter_st *)starter;

    VCore *vcore = start->core;
    _RETURN_UPON_ERROR(!vcore, "invalid vcore given for running", INVALID_VCORE);

    if (!is_valid_core(vcore))
    {
        destroy_vcore(vcore);
        int ret_val = create_vcore(vcore);
        if (ret_val) return ret_val;
    }

    const char *filename = start->bytecode_filename;
    if (filename && !load_file_to_core(vcore, filename))
        return BYTECODE_LOAD_ERR;


    return OK;
}

int create_vcore(VCore *vcore, size_t bytecode_size, size_t stack_capacity)
{
    assert(vcore);

    vcore->stack

    return OK;
}

void destroy_vcore(VCore *vcore)
{
    assert(vcore);

    vcore->stack->Dtor();
    vcore->return_ips->Dtor();
    free(vcore->bytecode);
    free(vcore->stack);
    free(vcore->return_ips);
    
    vcore->bytecode = NULL;
    vcore->bytecode_size = 0;
    vcore->return_ips = NULL;
    vcore->stack = NULL;
    
    for (int i = 0; i < REGISTER_NUMBER; i++)
        vcore->registers[i] = 0;
}

int load_file_to_core(VCore *vcore, const char *filename)
{
    assert(vcore);
    assert(is_valid_core(vcore));
    assert(filename);

    FILE *bytecode = fopen(filename, "rb");
    _RETURN_UPON_ERROR(!bytecode, "bytecode file open error", BYTECODE_LOAD_ERR, fclose(bytecode));

    size_t file_size = get_file_size(bytecode);
    _RETURN_UPON_ERROR(!file_size, "it seems that file is empty", BYTECODE_LOAD_ERR, fclose(bytecode));

    if (vcore->bytecode_size < file_size
            && resize_vcore_bytecode(vcore, file_size))
    {
        fclose(bytecode);
        return BYTECODE_LOAD_ERR;
    }

    if (fwrite(vcore->bytecode, file_size, sizeof(char), bytecode) < file_size)
    {
        LOG_ERR("file wasn't read fully");
        fclose(bytecode);
        return BYTECODE_LOAD_ERR;
    }

    fclose(bytecode);
    return OK;
}

bool is_valid_core(const VCore *vcore)
{
    assert(vcore);

    if (!vcore->stack) return false;
    if (!vcore->return_ips) return false;
    if (!vcore->bytecode) return false;

    return true;
}