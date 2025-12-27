#include "stack.h"

#define GENERATE_TYPED_SINGLE_VALUE_PRINT(type, format)                 \
template<>                                                              \
void Stack<type>::single_value_print(FILE *out_buff, const type *value) \
{                                                                       \
    fprintf(out_buff, format, *value);                                  \
}

GENERATE_TYPED_SINGLE_VALUE_PRINT(double, "%lg");
GENERATE_TYPED_SINGLE_VALUE_PRINT(float, "%g");
GENERATE_TYPED_SINGLE_VALUE_PRINT(int, "%d");
GENERATE_TYPED_SINGLE_VALUE_PRINT(unsigned int, "%u");
GENERATE_TYPED_SINGLE_VALUE_PRINT(char, "%d");
GENERATE_TYPED_SINGLE_VALUE_PRINT(unsigned char, "%u");
GENERATE_TYPED_SINGLE_VALUE_PRINT(long long int, "%lld");
GENERATE_TYPED_SINGLE_VALUE_PRINT(unsigned long long int, "%llu");
GENERATE_TYPED_SINGLE_VALUE_PRINT(size_t, "%lu");
