#include "stack.h"

template<>
void Stack<int>::data_print(FILE *filename, int *data_ptr)
{
    if (filename)
        fprintf(filename, "%d\n", *data_ptr);
}

template<>
void Stack<char>::data_print(FILE *filename, char * data_ptr)
{
    if (filename) 
        fprintf(filename, "%d\n", *data_ptr);
}

template<>
void Stack<double>::data_print(FILE *filename, double *data_ptr)
{
    if (filename)
        fprintf(filename, "%.2f\n", *data_ptr);
}

template<>
void Stack<long long int>::data_print(FILE *filename, long long int *data_ptr)
{
    if (filename)
        fprintf(filename, "%lld\n", *data_ptr);
}