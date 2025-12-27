#include <getopt.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "cmdline_options.h"
#include "log.h"

struct Options_t {
    const char *filename;
    size_t ram_amount;
};

static Options_t options = {.filename = NULL, .ram_amount = DEFAULT_RAM_AMOUNT};

void print_help_message();

int parse_cmdline_options(int argc, char *const argv[])
{
    assert(argv);

    LOG("parsing command line options");

    const struct option opts[] = 
    {
        {"input", required_argument, NULL, 'i'},
        {"ram-amount", required_argument, NULL, 'r'},
        {"help", no_argument, NULL, 'h'},
        {NULL, no_argument, NULL, 0}
    };

    int opt = 0;
    while ((opt = getopt_long(argc, argv, ":i:r:h", opts, NULL)) != -1)
    {
        switch (opt)
        {
        case 'i':
            options.filename = strdup(optarg);
            break;

        case 'r':
            options.ram_amount = (size_t)atoi(optarg);
            if (!options.ram_amount)
            {
                LOG_ERR("--ram-amount with bad value");
                printf("Option error: --ram-amount should be specified with number\n");

                return OPTIONS_BAD;
            }
        break;

        case ':':
            LOG_ERR("--ram-amount with no argument");
            printf("Option error: --ram-amount takes an argument\n");

            return OPTIONS_BAD;
            
        case '?':
            LOG_ERR("unknown argument");
            printf("Option error: unknown argument, please check commandline\n");
            [[fallthrough]];

        case 'h':
            print_help_message();

            return OPTIONS_BAD;

        default:
            LOG_ERR("critical error occured in %s:%s() at line %s", __FILE__, __func__, __LINE__);
            return OPTIONS_BAD;

        }
    }

    if (!options.filename)
    {
        LOG_ERR("input file options error");
        printf("Option error: you must input a bytecode file to virtual machine to run, please type '-h'\n");

        return OPTIONS_BAD;
    }

    LOG("options parsing complete");

    return OPTIONS_OK;
}

void print_help_message()
{
    printf(
        "This is SPU(virtual machine) with it's own architecture\n"
        "Usage:\n"
        "spu_machine --input={input_file_name} [options]\n"
        "Options:\n"
        "--input, -i [arg]: bytecode input filename\n"
        "--ram-amount, -r [num]: explicitly set size of SPU's RAM amount\n"
    );
}

size_t get_ram_amount()
{
    return options.ram_amount;
}

const char *get_filename()
{
    return options.filename;
}