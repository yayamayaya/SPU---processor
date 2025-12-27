#include <assert.h>
#include <stdarg.h>
#include <time.h>
#include <dirent.h>
#include <locale.h>
#include <stdio.h>
#include <wchar.h>
#include <sys/stat.h>

#include "stack.h"

#ifndef LOGGING_HPP
#define LOGGING_HPP

enum MessageType
{
    MSG = 0,
    ERR = 1
};

#define STACK_LOG(...)                                                               \
  do {                                                                         \
    log_msg(MSG, __VA_ARGS__);                                                 \
  } while (0)

#define VAL_LOG(arg, ...)                                                      \
do {                                                                           \
    log_with_arg(MSG, arg, __VA_ARGS__);                                       \
} while (0)

#define STACK_LOG_ERR(...)                                                           \
  do {                                                                         \
    log_msg(ERR, __VA_ARGS__);                                                 \
    fprintf(stderr, __VA_ARGS__);                                              \
  } while (0)

static const char log_dir_name[] = "logs";

static const size_t log_msg_size = 250;
static const char err_type[] = "ERROR";
static const char info_type[] = "INFO";

template<typename T>
typename Stack<T>::stack_err_t Stack<T>::log_init(const char *stack_log_filename) {
  DIR *log_dir = opendir(log_dir_name);
  if (!log_dir && (mkdir(log_dir_name, 0777) == -1)) {
    perror("Dir creation");
    return LOG_FILE_OPEN_ERR;
  }
  if (log_dir)
      closedir(log_dir);

  char log_path[100] = {0};
  snprintf(log_path, sizeof(log_path), "%s/%s", log_dir_name, stack_log_filename);

  logfile = fopen(log_path, "wb");
  if (!logfile) {
    printf("Stack error: couldn't open logfile '%s'", stack_log_filename);
    perror("");
    return LOG_FILE_OPEN_ERR;
  }

  setbuf(logfile, NULL);

  return 0;
}

template<typename T>
void Stack<T>::log_msg(const int option, const char *format, ...) {
  assert(option == MSG || option == ERR);
  if (!logfile)
      return;

  char log_msg[log_msg_size] = {0};

  va_list args = {};
  va_start(args, format);

  const char *log_type = (option == ERR) ? err_type : info_type;
  sprintf(log_msg, "[%ld] %s: %s\n", clock(), log_type,
           format);

  vfprintf(logfile, log_msg, args);

  va_end(args);
}

template<typename T>
void Stack<T>::log_with_arg(const int option, const T *arg, const char *format, ...) {
  assert(option == MSG || option == ERR);
  if (!logfile)
      return;

  char log_msg[log_msg_size] = {0};

  va_list args = {};
  va_start(args, format);

  const char *log_type = (option == ERR) ? err_type : info_type;
  sprintf(log_msg, "[%ld] %s: %s", clock(), log_type,
           format);

  vfprintf(logfile, log_msg, args);
  single_value_print(logfile, arg);
  fprintf(logfile, "\n");

  va_end(args);
}



#endif
