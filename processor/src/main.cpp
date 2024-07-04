#include "switch.h"
#include "file_reading.h"

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("[error]>>> not enough or too many arguments for programm call, enter the input file\n");
        return 101;
    }

    _OPEN_SPU_LOG();
    SPU_LOG("> reading a bytcode file\n");
    bytecode_t bytecode = {0};
    file_read((char **)&bytecode.bytecode, &bytecode.bytecode_size, argv[1]);

    SPU_LOG("> initialising SPU\n");
    processor_t processor = {};
    processor.stk.Ctor(10, "logs/data_stack.log");
    processor.return_ptrs.Ctor(5, "logs/return_stack.log");

    SPU_LOG(">starting command sequence\n");
    int error_number = 0;
    for (; bytecode.ip < bytecode.bytecode_size;)
    {
        error_number = do_command(&processor, &bytecode);
        if (error_number)
            break;
    }

    SPU_LOG("command sequence completed, ending programm\n");
    if (error_number != 1)
        SPU_LOG("> programm ended unsuccesfully, ending with error %d", error_number);

    processor.stk.Dtor();
    processor.return_ptrs.Dtor();
    free(bytecode.bytecode);
    _CLOSE_SPU_LOG();
    
    if (error_number == 1)
        return 0;
        
    return error_number;
}
