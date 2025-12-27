#include <stdlib.h>

#ifndef CMDLINE_OPTIONS
#define CMDLINE_OPTIONS

int parse_cmdline_options(int args, char *const argv[]);

size_t get_ram_amount();

const char *get_filename();

enum OPTIONS_PARSER_RESULT
{
    OPTIONS_OK  = 0,
    OPTIONS_BAD = 1,
};

#define DEFAULT_RAM_AMOUNT (1024)

#endif