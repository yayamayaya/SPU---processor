#ifndef LOG_FUNC
#define LOG_FUNC

enum LOG_ERRORS {
  FILE_OPEN_ERR = 11,
};

#ifdef DEBUG

#define LOG(...)                                                               \
  do {                                                                         \
    log_msg(0, L##__VA_ARGS__);                                                \
  } while (0)

#define LOG_ERR(...)                                                           \
  do {                                                                         \
    log_msg(1, L##__VA_ARGS__);                                                \
  } while (0)

#define LOG_CRITICAL()                                                                     \
  do {                                                                                     \
    LOG_ERR("critical error occured in %s:%s() at line %s", __FILE__, __func__, __LINE__)  \
  }   

#define _OPEN_LOG(arg)                                                         \
  do {                                                                         \
    log_init(arg);                                                             \
  } while (0)

#define _CLOSE_LOG()                                                           \
  do {                                                                         \
    close_log();                                                               \
  } while (0)

#else

#define LOG(...) while (0)

#define LOG_ERR(...) while (0)

#define LOG_CRITICAL() while (0)

#define _OPEN_LOG(arg) while (0)

#define _CLOSE_LOG() while (0)

#endif

int log_init(const char *log_name);

void log_msg(const int option, const wchar_t *format, ...);

void close_log();

#endif
