#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#ifndef STACK_HEADER
#define STACK_HEADER

#define CANARYPRINT(arg1, arg2) fprintf(fileName, ">> Canaries are %llX & %llX\n", arg1, arg2)
#define HASHPRINT(arg1)         fprintf(fileName, ">> Hash is: %llX\n", arg1)

typedef long long int class_canary_t;
typedef long long int canary_t;    
typedef long long int hash_t;

//#define SECURE

template <typename T>
class Stack
{
public:
    Stack(): logfile(NULL), data(NULL), size(0), capacity(0) {}
    ~Stack() {};
    Stack(const Stack<T> &st) =delete;
    Stack<T> operator= (const Stack<T> &st) =delete;

    int Ctor(const int stack_capacity, const char* logfile_name);
    int Dtor();
    int push(const T num);
    int pop(T *num);
    int print(int option);
    int verificator();
    int stk_realloc(const int num);
    T   get_data_on_pos(const int pos);
    T   *get_data_ptr();
    int get_stack_size();

private:
    static constexpr class_canary_t c_left_canary   = 0xABEBADED;
    static constexpr class_canary_t c_right_canary  = 0xDEDABEBA;

    static constexpr canary_t lcanary = 0xDEDDEAD;
    static constexpr canary_t rcanary = 0xFFABEBA;

    static constexpr unsigned char poison = 0xBD;

    hash_t hash_func();
    int poison_func(const int pos);
    int poison_check();
    int canary_check();
    int hash_check();

    canary_t *get_left_canary_ptr();
    hash_t *get_hash_number_ptr();
    canary_t *get_right_canary_ptr();

    void data_print(FILE *filename, T *data);
    void bin_print(FILE *filename, void *data_ptr, const int type_size);
    
    FILE *logfile;

    class_canary_t class_lcanary = c_left_canary;

    T* data;
    int size;
    int capacity;

    class_canary_t class_rcanary = c_right_canary;
};

enum DEBUGGING
{
    STK_CTED = 0,
    CAPACITY_ZERO = 1,
    STK_PUSH = 4,
    STK_POP = 5,
    STK_EMPTY = 6,
    CAPACITY_UP = 7,
    CAPACITY_DOWN = 8,
    STK_DTED = 9,
    STK_KILL = -1
};

enum STACK_ERRS
{
    STK_MEM_ALC_ERR = 515,
    STK_MEM_RLC_ERR = 516,
    PSN_ERR = 10,
    CANARY_ERR = 11,
    HASH_ERR = 12,
    NULL_PTR = 13,
    LOG_OPEN_ERR = 20

};

enum STK_REALLOC
{
    DOWN = 0, 
    UP = 1
};

enum STK_PRINT_OPTIONS
{
    IN_FILE = 0,
    IN_CONSOLE = 1
};

#ifdef SECURE

#define STACK_SIZE                      capacity * sizeof(T) + 2 * sizeof(canary_t) + sizeof(hash_t)
#define STACK_DATA_POSITION             sizeof(canary_t) + sizeof(hash_t)
#define _HASH_REFRESH()                 *(get_hash_number_ptr()) = hash_func()

#else

#define STACK_SIZE                      (size_t)capacity * sizeof(T)
#define STACK_DATA_POSITION             0
#define _HASH_REFRESH()                 do {} while (0)

#endif

#include "stack_log.h"

#include "stack_itern_func.hpp"
#include "stack_ext_func.hpp"
#endif