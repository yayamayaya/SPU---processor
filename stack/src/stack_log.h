#ifndef STACK_LOGGING
#define STACK_LOGGING

#ifdef DEBUG

#define _OPEN_STACK_LOG()\
    if (!logfile_name)                                                                              \
    {                                                                                               \
        fprintf(stderr, "[error]>>> logfile name for stack is NULL\n");                             \
        return 1;                                                                                   \
    }                                                                                               \
    logfile = fopen(logfile_name, "w");                                                             \
    if (logfile == NULL)                                                                            \
    {                                                                                               \
        fprintf(stderr, "[error]>>> can't open stack log\n");                                       \
        return 1;                                                                                   \
    }                                                                                               

#define _STACK_LOG(...)                                                                             \
    if (logfile)                                                                                    \
        fprintf(logfile, __VA_ARGS__)                                                               \

#define _LOG_FILE_CLOSE()                                                                           \
    if (logfile)                                                                                    \
        fclose(logfile);                                                                            \
    logfile = NULL


#else

#define _OPEN_STACK_LOG()       do {} while (0)

#define _STACK_LOG(...)         do {} while (0)

#define _LOG_FILE_CLOSE()       do {} while (0)

#endif

#define _DUMP_TO_LOGFILE()                                                                          \
    if      (!option && logfile)                                                                    \
        fileName = logfile;                                                                         \
    else if (!option)                                                                               \
        return 1
    
    
#endif