#include "log.h"
#include "cmdline_options.h"
#include "virtual_machine.h"

int main(int argc, char *const argv[])
{
    _OPEN_LOG("SPU_runtime.log");

    int return_value = parse_cmdline_options(argc, argv);

    if (!return_value)
        return_value = run_virtual_machine();

    _CLOSE_LOG();
    return return_value;
}