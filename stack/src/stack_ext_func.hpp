#include "stack.h"

#ifndef STACK
#define STACK

template <typename T>
T *Stack<T>:: get_data_ptr()
{
    return (T *)((char *)data + STACK_DATA_POSITION);
}

template <typename T>
int Stack<T>::Ctor(const int stack_capacity, const char* logfile_name)
{
    if (!stack_capacity)
    {
        fprintf(stderr, "[error]>>> stack capacity can't be 0\n");
        return 1;
    }
    _OPEN_STACK_LOG();

    _STACK_LOG("> constructing stack\n");
    _STACK_LOG("> allocating memory for the stack:\n");
    capacity = stack_capacity;
    data = (T *)calloc(STACK_SIZE, 1);
    if (!data)
    {
        _STACK_LOG("[error]>>> stack memory allocation error\n");
        _LOG_FILE_CLOSE();
        return STK_MEM_ALC_ERR;
    }
    
#ifdef SECURE
    *get_left_canary_ptr() = lcanary;
    *get_hash_number_ptr() = hash_func();
    *get_right_canary_ptr() = rcanary;
#endif

    poison_func(0);   
    if (logfile)
        bin_print(logfile, get_data_ptr(), sizeof(T));

    _STACK_LOG("> stack %p was constructed with the capacity: %d\n", data, capacity); 
    return verificator();
}

template <typename T>
int Stack<T>::Dtor()
{
    int err_num = verificator();
    
    capacity = 0;
    size = 0;

    free(data);
    data = NULL;

    _STACK_LOG("> stack was destructed\n");
    _LOG_FILE_CLOSE();

    return err_num;
}

template <typename T>
int Stack<T>::push(const T num)
{
    int err_num = verificator();
    if (err_num)
        return err_num;

    if (size == capacity)
        if(stk_realloc(UP))
            return STK_MEM_RLC_ERR;
    
    *(get_data_ptr() + size) = num;
    _STACK_LOG("> value was pushed on pos: %d with pointer <%p>\n", size + 1, get_data_ptr() + size);
    _STACK_LOG("> data : \n");
    data_print(logfile, get_data_ptr() + size);

    size++;
    
    _HASH_REFRESH();
    
    return verificator();
}

template <typename T>
int Stack<T>::pop(T *num)
{    
    int err_num = verificator();
    if (err_num)
        return err_num;

    if (size == 0)
    {
        _STACK_LOG(">> stack is empty, nothing to pop\n");
        return STK_EMPTY;
    }

    size--;   
    if (num != NULL)
        *num = *(get_data_ptr() + size);
    
    poison_func(size);

    _HASH_REFRESH();

    _STACK_LOG("> data was popped from the stack\n");
    return verificator();
}

template <typename T>
int Stack<T>::print(int option)
{
    FILE *fileName = stdout;
    setbuf(stdout, NULL);    
    _DUMP_TO_LOGFILE();

    int err_num = verificator();
    if (err_num)
        return err_num;

    fprintf(fileName, "--------------------------------------------------------------------------------\n");
    fprintf(fileName, "\n>> Stack dump:\n");
    fprintf(fileName, "\n>> Size: %d", size);
    fprintf(fileName, "\n>> Capacity: %d", capacity);
    fprintf(fileName, "\n>> Current data in Stack:\n");    
    for (int i = 0; i < size; i++)
    {
        fprintf(fileName, "%d) ", i + 1);
        data_print(fileName, get_data_ptr() + i);        
    }

#ifdef SECURE
    fprintf(fileName, ">> Poison is: ");
    fprintf(fileName, "%d\n", poison);
    CANARYPRINT(*(get_left_canary_ptr()), *(get_right_canary_ptr()));
    fprintf(fileName, ">> Class canaries:\n");
    CANARYPRINT(class_lcanary, class_rcanary);
    HASHPRINT(*(get_hash_number_ptr()));
#endif

    fprintf(fileName, "--------------------------------------------------------------------------------\n");

    return 0;
}

template <typename T>
int Stack<T>::stk_realloc(const int num)
{
    assert(num == UP || num == DOWN);

    int err_num = verificator();
    if (err_num)
        return err_num;

    if (num == UP)
    {
        capacity *= 2;
        _STACK_LOG("> stack capacity doubled\n");
    }
    else if(num == DOWN)
    {
        if (capacity == 1)
        {
            _STACK_LOG("[error]>>> stack memory reallocation error, capacity turned to 0\n");
            return STK_MEM_RLC_ERR;
        }
        
        capacity /= 2;
        _STACK_LOG("> capacity halfed\n");
    }

    T *mem_holder = (T *)realloc(data, STACK_SIZE);
    if (!mem_holder)
    {
        _STACK_LOG("[error]>>> stack memory reallocation error\n");
        return STK_MEM_RLC_ERR;
    }
    data = mem_holder;
    
#ifdef SECURE
    *get_right_canary_ptr() = rcanary;
#endif

    if (num == UP)
        if (poison_func(size))
        return PSN_ERR;
    
    _HASH_REFRESH();

    return verificator();
}

template <typename T>
T Stack<T>::get_data_on_pos(const int pos)
{
    return *(get_data_ptr() + pos);
}

template <typename T>
int Stack<T>::get_stack_size()
{
    return size;
}

#endif