#include "SPU_logs.h"

static FILE *log_file = NULL;

int SPU_open_log()
{
    log_file = fopen("logs/SPU.log", "wb");
    if (!log_file)
    {
        printf("[error]> couldn't open SPU logfile.\n");
        return 32;
    }

    setbuf(log_file, NULL);
    return 0;
}

void SPU_close_log()
{
    if (log_file)
        fclose(log_file);
}

void log(const char *format, ...)
{
    va_list args = {};
    va_start(args, format);
    if (log_file)
        vfprintf(log_file, format, args);
    va_end(args);
}