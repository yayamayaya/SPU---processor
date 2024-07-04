#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#ifndef SPU_LOGS
#define SPU_LOGS

int SPU_open_log();
void SPU_close_log();
void log(const char *format, ...);

#define _OPEN_SPU_LOG()             \
    if(SPU_open_log())              \
        return 32

#define _CLOSE_SPU_LOG()            \
    SPU_close_log()

#define SPU_LOG(...)                \
    log(__VA_ARGS__)

#define _CLEAR_LOGS()               \
    system("rm logs/*")

#endif