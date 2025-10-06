#ifndef OPTIONS_PARSING
#define OPTIONS_PARSING

enum OPTIONS_STATUS {
  OPTIONS_OK  = 0,
  OPTIONS_BAD = 1,
};

enum OPTIONS_STATUS parse_options(const int argc, char *const argv[]);

const char *input_file_name();

const char *output_file_name();

bool skip_codegen();

bool tokens_dump_enabled();

bool label_dump_enabled();

void free_files();

#endif