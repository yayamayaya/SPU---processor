#include <source_location>
#include <stdexcept>
#include <assert.h>

#ifndef STACK_EXTERNAL_FUNCTIONS_HPP
#define STACK_EXTERNAL_FUNCTIONS_HPP

#include "stack.h"

#define ERROR_WITH_LOCATION(...)                                            \
    STACK_LOG_ERR(__VA_ARGS__);                                             \
    fprintf(stderr, "\nError occured in '%s:%u' in function '%s'\n",        \
        location.file_name(), location.line(), location.function_name())

#ifdef SECURED
    #define UPDATE_HASH()           update_hash()
    #define UPDATE_POISON(from, to) update_poison(from, to)
    #define SET_RIGHT_CANARY()      set_data_right_canary(data_right_canary_value)
#else
    #define UPDATE_HASH()           do {} while(0)
    #define UPDATE_POISON(from, to) do {} while(0)
    #define SET_RIGHT_CANARY()      do {} while(0)
#endif

template <typename T>
Stack<T>::Stack(const size_t cap,
    const std::source_location& location): logfile(NULL), hash(0), error_mask(0), data(NULL), size(0), capacity(0)
{
    if (cap == 0)
    {
        ERROR_WITH_LOCATION("Stack error: stack capacity can't be 0");
        throw std::invalid_argument("Stack capacity can't be 0");
    }

    data = (T *)calloc(1, DATA_SIZE);
    if (!data)
    {
        STACK_LOG_ERR("Couldn't allocate memory while initializing stack");
        throw std::runtime_error("Memory allocation error");
    }

    size = 0;
    capacity = cap;

#ifdef SECURED
    STACK_LOG("SECURED stack is enabled, setting stack protection");

    set_data_left_canary(data_left_canary_value);
    set_data_right_canary(data_right_canary_value);
    update_hash();
    fill_poison();
#endif

    STACK_LOG("Stack constructed");
    if (verificator())
        throw std::runtime_error("Verificator error");
}

template <typename T>
Stack<T>::~Stack()
{
    verificator();

    STACK_LOG("Stack work is finished");
    free(data);
    if (logfile)
        fclose(logfile);
}

template<typename T>
size_t Stack<T>::stack_size()
{
    STACK_LOG("Getting stack size");
    return size;
}

template<typename T>
T* Stack<T>::at(const size_t ind, const std::source_location& location)
{
    STACK_LOG("Getting value at index %lu", ind);
    if (ind >= size)
    {
        ERROR_WITH_LOCATION("Stack error: index out of bounds");
        return NULL;
    }

    VAL_LOG((DATA_PTR + ind), "Gotten value: ");

    return DATA_PTR + ind;
}

template<typename T>
T Stack<T>::pop(const std::source_location& location)
{
    if (verificator())
        throw std::logic_error("Stack verificator did not pass in pop");

    if (size == 0)
    {
        ERROR_WITH_LOCATION("Stack error: you can't pop the value from empty stack");
        throw std::logic_error("empty stack poppin'");
    }

    STACK_LOG("Popping value from stack");

    size--;
    T popped = *(DATA_PTR + size);

    UPDATE_POISON(size, 1);
    UPDATE_HASH();
    VAL_LOG(&popped, "Popped value: ");

    verificator();
    return popped;
}

template<typename T>
void Stack<T>::push(const T value)
{
    if (verificator())
        return;

    STACK_LOG("Pushing value to stack");

    if (size == capacity && resize(capacity * 2))
        return;

    *(DATA_PTR + size) = value;
    size++;

    UPDATE_HASH();
    VAL_LOG(&value, "Pushed value: ");

    verificator();
}

template<typename T>
typename Stack<T>::stack_err_t Stack<T>::resize(const size_t new_capacity, \
        const std::source_location& location)
{
    if (verificator())
        return error_mask;

    if (new_capacity == 0)
    {
        ERROR_WITH_LOCATION("Stack resize error: stack capacity can't be 0");
        error_mask |= STK_MEM_RLC_ERR;
        return STK_MEM_RLC_ERR;
    }

    STACK_LOG("resizing stack to capacity = %lu", new_capacity);

    size_t cap = new_capacity;
    T *tmp_data = (T *)realloc(data, DATA_SIZE);
    if (!tmp_data)
    {
        ERROR_WITH_LOCATION("Stack resize error: realloc finished with error");
        error_mask |= STK_MEM_RLC_ERR;
        return STK_MEM_RLC_ERR;
    }

    data = tmp_data;
    capacity = cap;

    SET_RIGHT_CANARY();
    UPDATE_POISON(size, capacity - size);
    UPDATE_HASH();

    STACK_LOG("stack resized");

    return verificator();
}

template <typename T>
void Stack<T>::print(FILE *out_buff)
{
    assert(out_buff);

    if (error_mask)
        fprintf(out_buff, "Errors occured in stack work\n");

    fprintf(out_buff, "---------------- Stack dump -----------------\n");

    fprintf(out_buff, "Current size = %lu\n", size);
    fprintf(out_buff, "Capacity = %lu\n", capacity);

    if ((out_buff == stderr) || (out_buff == logfile))
        extended_print(out_buff);
    else
        data_print(out_buff);

    fprintf(out_buff, "---------------------------------------------\n");
}

template <typename T>
void Stack<T>::log_print(const std::source_location& location)
{
    if (!logfile)
    {
        ERROR_WITH_LOCATION("Stack error: cannot print in logfile, please open logfile before that");
        return;
    }

    print(logfile);
}

template<typename T>
void Stack<T>::enable_stack_log(const char *stack_log_filename)
{
    assert(stack_log_filename);

    error_mask |= log_init(stack_log_filename);

    UPDATE_HASH();
    verificator();

    STACK_LOG("Stack log created");
}

template <typename T>
typename Stack<T>::stack_err_t Stack<T>::get_stack_error()
{
    return error_mask;
}

#undef PRINTLN_WITH_LOCATION

#endif
