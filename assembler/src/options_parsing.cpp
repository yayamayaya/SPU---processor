#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "log.h"
#include "options_parsing.h"

void print_help_message();

typedef struct files {
  char *input;
  char *output;
  bool skip_codegen;
  bool dump_tokens;
  bool dump_labels;
} Options;

static Options ASM_OPTIONS = {0};

enum OPTIONS_STATUS parse_options(const int argc, char *const argv[]) {
  assert(argv);

  Options res = {0};

  const struct option longopts[] = {
    {"help", no_argument, NULL, 'h'},
    {"input", required_argument, NULL, 'i'},
    {"output", required_argument, NULL, 'o'},
    {"skip-codegen", optional_argument, NULL, 's'},
    {"token-dump", optional_argument, NULL, 't'},
    {"label-dump", optional_argument, NULL, 'l'},
    {NULL, 0, NULL, '\0'},
  };

  LOG("Parsing assembly options");
  int opt = 0;
  while ((opt = getopt_long(argc, argv, "lsthi:o:", longopts, NULL)) != -1) {
    switch (opt) {

    case '?':
      printf("Unknown argument, please rewind arguments\n");
      LOG_ERR("Unknown argument error");
      [[fallthrough]];
    case 'h':
      print_help_message();
      exit(0);

    case 'i':
      res.input = strdup(optarg);
      if (!res.input)
      {
        LOG_ERR("Error while copying a string: %s", strerror(errno));
        return OPTIONS_BAD;
      }

      break;

    case 'o':
      res.output = strdup(optarg);
      if (!res.output) {
        LOG_ERR("Error while copying a string: %s", strerror(errno));
        return OPTIONS_BAD;
      }

      break;

    case 's':
      res.skip_codegen = true;
      break;

    case 't':
      res.dump_tokens = true;
      break;

    case 'l':
      res.dump_labels = true;
      break;

    default:
      LOG_ERR("FATAL: default case shouldn't been reached");
      return OPTIONS_BAD;
    }
  }

  ASM_OPTIONS = res;

  if (!res.skip_codegen && !res.output)
  {
    printf("No output file was given, '-o' option to write output file, or '-s' to skip bytecode generation\n");
    free_files();

    return OPTIONS_BAD;
  }

  if (!res.input)
  {
    printf("No input file was given, '-i' is required\n");
    free_files();

    return OPTIONS_BAD;
  }

  return OPTIONS_OK;
}

const char *input_file_name() 
{
  return ASM_OPTIONS.input;
}

const char *output_file_name() 
{
  return ASM_OPTIONS.output;
}

bool skip_codegen() 
{
  return ASM_OPTIONS.skip_codegen;
}

bool tokens_dump_enabled() 
{
  return ASM_OPTIONS.dump_tokens;
}

bool label_dump_enabled()
{
  return ASM_OPTIONS.dump_labels;
}

void print_help_message() {
  printf("Own assembly language compiler dedicated for this SPU\n");

  printf("Usage:\n");
  printf("--help,         -h: show this message\n");
  printf("--input[req],   -i: pass input file\n");
  printf("--output,       -o: pass output file\n");
  printf("--skip-codegen, -s: check syntax, skipping codegen\n");
  printf("--token-dump,   -t: dump all tokens produced from file\n");
  printf("--label-dump,   -l: dump all labels found in file\n");
}

void free_files() {
  free(ASM_OPTIONS.input);
  free(ASM_OPTIONS.output);
}
