#include "log.h"
#include "tokenization.h"
#include "compiler.h"
#include "asm_arrays.h"
#include "options_parsing.h"

int main(int argc, char *const argv[])
{
    _OPEN_LOG("compiler.log");

    int ret_val = parse_options(argc, argv);
    if (ret_val) {
        printf("Please check your options, type '-h' for help\n");

        _CLOSE_LOG();
        return ret_val;
    }

    AsmArrays arrays = {};
    int error = allocate_arrays_memory(&arrays);
    if (error)
        goto end;

    error = tokenize(&arrays.tokens);
    if (error)
        goto end;

    error = Compiler(&arrays);
    if (error)
        goto end;

    if (arrays.labels.get_stack_size())
    {
        arrays.bytecode.ip = 0;
        error = Compiler(&arrays);
        if (error)
            goto end;
    }

    if (label_dump_enabled())
        label_table_print(&arrays.labels);

    if (!skip_codegen())
        error = bytecode_to_file(output_file_name(), &arrays.bytecode);

end:
    free_arrays_memory(&arrays);
    free_files();
    _CLOSE_LOG();
    
    return error;
}
