#ifndef FILE_READING
#define FILE_READING

#include <stdlib.h>

enum FILE_READING_ERRORS {
  READ_FILE_OPN_ERR = 1313,
  EMPTY_FILE_ERR = 1314,
  FILE_MEM_ALC_ERR = 1315,
  NOT_READ_FULLY = 1316,
};

int file_read(char **buff, size_t *file_size, const char *file_name);

#endif
