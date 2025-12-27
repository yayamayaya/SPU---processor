#include <assert.h>
#include <cstdio>

#ifndef STACK_INTERNAL_FUNCTIONS_HPP
#define STACK_INTERNAL_FUNCTIONS_HPP

#include "stack.h"

template<typename T>
T *Stack<T>::data_ptr()
{
    return DATA_PTR;
}

template<typename T>
bool Stack<T>::is_class_left_canary_valid()
{
    return class_left_canary == class_left_canary_value;
}

template<typename T>
bool Stack<T>::is_class_right_canary_valid()
{
    return class_right_canary == class_right_canary_value;
}

template<typename T>
typename Stack<T>::canary_t *Stack<T>::data_left_canary_ptr()
{
    assert(data);

    return (canary_t *)data;
}

template<typename T>
typename Stack<T>::canary_t *Stack<T>::data_right_canary_ptr()
{
    assert(data);

    return (canary_t *)((char *)data + sizeof(canary_t) + capacity * sizeof(T));
}

template<typename T>
void Stack<T>::set_data_left_canary(const canary_t canary)
{
    *data_left_canary_ptr() = canary;
}

template<typename T>
void Stack<T>::set_data_right_canary(const canary_t canary)
{
    *data_right_canary_ptr() = canary;
}

template<typename T>
bool Stack<T>::is_data_left_canary_valid()
{
    return (*data_left_canary_ptr() == data_left_canary_value);
}

template<typename T>
bool Stack<T>::is_data_right_canary_valid()
{
    return (*data_right_canary_ptr() == data_right_canary_value);
}

template<typename T>
typename Stack<T>::hash_t Stack<T>::evaluate_hash()
{
    hash_t eval_hash = 0;

    eval_hash += (hash_t)logfile;

    eval_hash += capacity << 6;
    eval_hash += size << 7;

    unsigned char *byte_data = (unsigned char *)data_ptr();

    for (size_t i = 0; i < size * sizeof(T); i++)
        eval_hash += i * byte_data[i];

    return eval_hash;
}

template<typename T>
void Stack<T>::update_hash()
{
    hash = evaluate_hash();
}

template<typename T>
bool Stack<T>::is_hash_valid()
{
    return hash == evaluate_hash();
}

template<typename T>
void Stack<T>::fill_poison()
{
    update_poison(size, capacity - size);
}

template<typename T>
bool Stack<T>::is_poisoned()
{
    poison_t *byte_data = (poison_t *)data_ptr();

    for (size_t i = size * sizeof(T); i < capacity * sizeof(T); i++)
        if (byte_data[i] != poison_value)
            return false;

    return true;
}

template<typename T>
void Stack<T>::update_poison(const size_t from, const size_t amount)
{
    assert(from + amount <= capacity);
    poison_t *byte_data = (poison_t *)data_ptr();

    for (size_t i = from * sizeof(T); i < (from + amount) * sizeof(T); i++)
        byte_data[i] = poison_value;
}

#define CHECK_FOR_ERROR(condition, error_code) \
    if (condition)                             \
        error |= error_code

template<typename T>
typename Stack<T>::stack_err_t Stack<T>::verificator()
{
    stack_err_t error = 0;
    stack_err_t to_print = 0;

    STACK_LOG("Verifying stack");

    CHECK_FOR_ERROR(capacity == 0, ZERO_CAPACITY_ERR);
    CHECK_FOR_ERROR(data == NULL, NULL_DATA_ERR);

#ifdef SECURED
    CHECK_FOR_ERROR(!is_class_left_canary_valid(), LEFT_CLASS_CANARY_ERR);
    CHECK_FOR_ERROR(!is_class_right_canary_valid(), RIGHT_CLASS_CANARY_ERR);
    CHECK_FOR_ERROR(!is_data_left_canary_valid(), LEFT_DATA_CANARY_ERR);
    CHECK_FOR_ERROR(!is_data_right_canary_valid(), RIGHT_DATA_CANARY_ERR);
    CHECK_FOR_ERROR(!is_hash_valid(), HASH_VALUE_ERR);
    CHECK_FOR_ERROR(!is_poisoned(), POISON_VALUE_ERR);
#endif

    to_print    = error ^ error_mask;
    error_mask |= error;

    if (to_print)
        print(stderr);

    STACK_LOG("Verifying complete");
    return error;
}

#undef CHECK_FOR_ERROR

#define ERROR_OR_NEWLINE(condition, message)    \
    if (condition)                              \
        fprintf(out_buff, "!" message);         \
    fprintf(out_buff, "\n");

#define LINE_UP_PRINT(condition, message)           \
    if (out_buff == stdout || out_buff == stderr)   \
        fprintf(out_buff, "\x1b[1A\x1b[1000C");     \
    if (condition)                                  \
        fprintf(out_buff, "!" message);             \
    if (out_buff == stdout || out_buff == stderr)   \
        fprintf(out_buff, "\n");

template<typename T>
void Stack<T>::extended_print(FILE *out_buff)
{
    stack_err_t error = error_mask;

    if (out_buff == stdout || out_buff == stderr)
    {
        fprintf(out_buff, "\x1b[1A");
        fprintf(out_buff, "\x1b[1000C");
    }
    LINE_UP_PRINT(error & ZERO_CAPACITY_ERR, "CAPACITY IS ZERO");
    LINE_UP_PRINT(error & STK_MEM_RLC_ERR, "CAPACITY FAILED TO REALLOC");

    fprintf(out_buff, "Logfile pointer = %p ", logfile);
    ERROR_OR_NEWLINE(error & LOG_FILE_OPEN_ERR, "LOG FILE OPENING ERROR");

#ifdef SECURED
    fprintf(out_buff, "Class left canary = 0x%llX ", class_left_canary);
    ERROR_OR_NEWLINE(error & LEFT_CLASS_CANARY_ERR, "BAD CANARY");

    fprintf(out_buff, "Class right canary = 0x%llX ", class_right_canary);
    ERROR_OR_NEWLINE(error & RIGHT_CLASS_CANARY_ERR, "BAD CANARY");

    fprintf(out_buff, "Data left canary = 0x%llX ", *data_left_canary_ptr());
    ERROR_OR_NEWLINE(error & RIGHT_CLASS_CANARY_ERR, "BAD CANARY");

    fprintf(out_buff, "Data right canary = 0x%llX ", *data_right_canary_ptr());
    ERROR_OR_NEWLINE(error & RIGHT_DATA_CANARY_ERR, "BAD CANARY");

    fprintf(out_buff, "Hash value = 0x%llX ", evaluate_hash());
    ERROR_OR_NEWLINE(error & HASH_VALUE_ERR, "BAD HASH");
#endif

    fprintf(out_buff, "Data pointer = %p ", data);
    ERROR_OR_NEWLINE(error & NULL_DATA_ERR, "UNINITIALIZED DATA");

    data_print(out_buff);

    if (error & POISON_VALUE_ERR)
        bad_poison_print(out_buff);
}

#undef ERROR_OR_NEWLINE
#undef LINE_UP_PRINT

template<typename T>
void Stack<T>::data_print(FILE *out_buff)
{
    for (size_t i = 0; i < size; i++)
    {
        fprintf(out_buff, "Value[%lu] = ", i);
        single_value_print(out_buff, at(i));
        fprintf(out_buff, "\n");
    }
}

template<typename T>
void Stack<T>::bad_poison_print(FILE *out_buff)
{
    poison_t *byte_data = (poison_t *)data_ptr();

    for (size_t i = size * sizeof(T); i < capacity * sizeof(T); i++)
    {
        if (byte_data[i] != poison_value)
            fprintf(out_buff, "POISON[%lu] = 0x%X !BAD POISON\n", i % sizeof(T), byte_data[i]);
    }
}

template<typename T>
void Stack<T>::single_value_print(FILE *out_buff, const T *value)
{
    const unsigned char *byted_value = (const unsigned char *)(value);

    fprintf(out_buff, "0x ");
    for (size_t i = 0; i < sizeof(T); i++)
        fprintf(out_buff, "%02X ", byted_value[i]);
}

#define GEN_SINGLE_VALUE_PRINT_SPECIALIZATION(type)                         \
template<>                                                                  \
void Stack<type>::single_value_print(FILE *out_buff, const type *value)

GEN_SINGLE_VALUE_PRINT_SPECIALIZATION(double);
GEN_SINGLE_VALUE_PRINT_SPECIALIZATION(float);
GEN_SINGLE_VALUE_PRINT_SPECIALIZATION(int);
GEN_SINGLE_VALUE_PRINT_SPECIALIZATION(unsigned int);
GEN_SINGLE_VALUE_PRINT_SPECIALIZATION(char);
GEN_SINGLE_VALUE_PRINT_SPECIALIZATION(unsigned char);
GEN_SINGLE_VALUE_PRINT_SPECIALIZATION(long long int);
GEN_SINGLE_VALUE_PRINT_SPECIALIZATION(unsigned long long int);
GEN_SINGLE_VALUE_PRINT_SPECIALIZATION(size_t);

#undef GEN_SINGLE_VALUE_PRINT_SPECIALIZATION

#endif
