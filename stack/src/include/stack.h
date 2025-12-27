#include <stdlib.h>
#include <stdio.h>
#include <source_location>

#ifndef STACK_H
#define STACK_H

#define LOCATION_ARGUMENT const std::source_location& location = std::source_location::current()

template <typename T>
class Stack
{
public :
    using stack_err_t   = unsigned int;

private:
    using canary_t      = long long unsigned int;
    using hash_t        = long long unsigned int;
    using poison_t      = unsigned char;

    static constexpr size_t   default_stack_capacity   = 16;

    static constexpr canary_t class_left_canary_value  = 0xDEDABEBA;
    static constexpr canary_t class_right_canary_value = 0xABEBADED;

    static constexpr canary_t data_left_canary_value   = 0xFFACEACE;
    static constexpr canary_t data_right_canary_value  = 0xDEDFACEL;

    static constexpr poison_t poison_value             = 0xAB;

    canary_t class_left_canary = class_left_canary_value;

    FILE *logfile;

    hash_t hash;
    stack_err_t error_mask;

    T *data;
    size_t size;
    size_t capacity;

    canary_t class_right_canary = class_right_canary_value;

    T *data_ptr();

    bool is_class_left_canary_valid();
    bool is_class_right_canary_valid();

    canary_t *data_left_canary_ptr();
    canary_t *data_right_canary_ptr();
    void set_data_left_canary(const canary_t canary);
    void set_data_right_canary(const canary_t canary);
    bool is_data_left_canary_valid();
    bool is_data_right_canary_valid();

    hash_t evaluate_hash();
    void update_hash();
    bool is_hash_valid();

    void fill_poison();
    void update_poison(const size_t from, const size_t amount);
    bool is_poisoned();

    stack_err_t verificator();
    void extended_print(FILE *out_buff);
    void data_print(FILE *out_buff);
    void bad_poison_print(FILE *out_buff);
    void single_value_print(FILE *out_buff, const T *value);

    stack_err_t log_init(const char *stack_log_filename);
    void log_msg(const int option, const char *format, ...);
    void log_with_arg(const int option, const T *arg, const char *format, ...);

public:
    enum StackErrors
    {
        ZERO_CAPACITY_ERR       = 0x01,
        NULL_DATA_ERR           = 0x02,
        LEFT_CLASS_CANARY_ERR   = 0x04,
        RIGHT_CLASS_CANARY_ERR  = 0x08,
        LEFT_DATA_CANARY_ERR    = 0x10,
        RIGHT_DATA_CANARY_ERR   = 0x20,
        HASH_VALUE_ERR          = 0x40,
        POISON_VALUE_ERR        = 0x80,
        LOG_FILE_OPEN_ERR       = 0x100,
        STK_MEM_RLC_ERR         = 0x200,
    };

    Stack(const unsigned long cap = default_stack_capacity, LOCATION_ARGUMENT);
    ~Stack();

    size_t stack_size();
    T* at(const size_t ind, LOCATION_ARGUMENT);
    T pop(LOCATION_ARGUMENT);
    void push(const T value);
    stack_err_t resize(const size_t new_capacity, LOCATION_ARGUMENT);

    void enable_stack_log(const char *stack_log_filename);
    stack_err_t get_stack_error();
    void print(FILE *out_buff = stdout);
    void log_print(LOCATION_ARGUMENT);

    Stack(const Stack<T> &copy) =delete;
    Stack<T> operator=(const Stack<T> &copy) =delete;
};

#undef LOCATION_ARGUMENT

/* Secure dependened macros */
#ifdef SECURED
    #define DATA_SIZE 2 * sizeof(canary_t) + cap * sizeof(T)
    #define DATA_PTR  (T *)((char *)data + sizeof(canary_t))
#else
    #define DATA_SIZE cap * sizeof(T)
    #define DATA_PTR  (data)
#endif

#include "../logging.hpp"

#include "../stack_external_functions.hpp"
#include "../stack_internal_functions.hpp"

#undef STACK_LOG
#undef STACK_LOG_ERR
#undef DATA_PTR
#undef DATA_SIZE

#endif
