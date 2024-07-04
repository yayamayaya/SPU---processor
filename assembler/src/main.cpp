#include "tokenization.h"
#include "compiler.h"
#include "asm.h"
#include "asm_arrays.h"

int main(int argc, char const *argv[])
{
    _CLEAR_LOGS();
    
    if (argc != 3)
    {
        printf("[error]>>> not enough arguments for programm call, enter the input and ouptut files\n");
        return 101;
    }

    int err_code = 0;
    asm_arrays arrays = {0};
    
    int error = tokenize(&arrays.tokens, argv[2]);
    if (error)
        return error;

    err_code = allocate_arrays_memory(&arrays);
    if (err_code)
        return err_code;
    

    err_code = Compiler(&arrays.tokens, &arrays.bytecode, &arrays.labels);
    if (err_code)
    {
        free_arrays_memory(&arrays);
        return err_code;
    }

    if (arrays.labels.get_stack_size())
    {
        arrays.bytecode.ip = 0;
        err_code = Compiler(&arrays.tokens, &arrays.bytecode, &arrays.labels);
        if (err_code)
        {
            free_arrays_memory(&arrays);
            return err_code;
        }
    }

    arrays.labels.print(IN_FILE);
    
    err_code = bytecode_to_file(argv[1], &arrays.bytecode);
    
    free_arrays_memory(&arrays);
    return err_code;
}
