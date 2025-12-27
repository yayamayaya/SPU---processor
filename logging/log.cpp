#include <cassert>
#include <cstdarg>
#include <ctime>
#include <dirent.h>
#include <locale.h>
#include <stdio.h>
#include <sys/stat.h>
#include <wchar.h>

#include "log.h"

static FILE *log_file = NULL;
static const char log_dir_name[] = "logs";

static const size_t log_msg_size = 250;
static const char err_type[] = "ERROR";
static const char info_type[] = "INFO";

enum MessageType
{
  MSG = 0,
  ERR = 1
};

int log_init(const char *log_name) {
  DIR *log_dir = opendir(log_dir_name);
  if (!log_dir && (mkdir(log_dir_name, 0777) == -1)) {
    perror("Dir creation");
    return FILE_OPEN_ERR;
  }
  if (log_dir)
      closedir(log_dir);

  char log_path[100] = {0};
  snprintf(log_path, sizeof(log_path), "%s/%s", log_dir_name, log_name);

  log_file = fopen(log_path, "wb");
  if (!log_file) {
    printf("Error: couldn't open logfile %s\n", log_name);
    return FILE_OPEN_ERR;
  }

  setlocale(LC_ALL, "ru_RU.UTF-8");
  setbuf(log_file, NULL);

  return 0;
}

void log_msg(const int option, const wchar_t *format, ...) {
  assert(option == MSG || option == ERR);

  wchar_t log_msg[log_msg_size] = {0};

  va_list args = {};
  va_start(args, format);

  const char *log_type = (option == ERR) ? err_type : info_type;
  swprintf(log_msg, sizeof(log_msg), L"[%ld] %s: %ls\n", clock(), log_type,
           format);

  if (log_file)
    vfwprintf(log_file, log_msg, args);

  va_end(args);
}

void close_log() {
  log_msg(MSG, L"closing logfile");
  if (log_file)
    fclose(log_file);

  log_file = NULL;
}
