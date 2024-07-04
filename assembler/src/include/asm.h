#ifndef ASM_H
#define ASM_H

typedef unsigned char byte_t;

typedef double data_t;
#define DATA_NSPEC      "%lf%n"
#define DATA_SPEC       "%lf"
#define DATA_PRINT_SPEC "%.02lf\n"

enum MACHINE_CODE
{
    PUSH_GRP        = 0x81,    

    ADD             = 0x02,     //Сложение
    SUB             = 0x03,     //Вычитание
    MULT            = 0x04,     //Умножение
    DIV             = 0x05,     //Деление
    LOG_E           = 0x11,
    LOG_NE          = 0x12,
    LOG_A           = 0x13,
    LOG_AE          = 0x14,
    LOG_B           = 0x15,
    LOG_BE          = 0x16,    
    SQRT            = 0x17,     //Квадратный корень
    POW             = 0x18,     //степень

    POP_GRP         = 0x82,
    
    IN              = 0x06,     //Ввод с консоли   

    MOV_GRP         = 0x93,     //Команда mov

    OUT             = 0x07,     //Вывод
    OUTI            = 0x19,     //Одиночный вывод

    HALT            = 0x0F,     //Завершение 

    NOCMD           = 0xF0,

    //GRAPHICS        = ,    

    CALL            = 0x84,     //"Вызов"
    
    JMP             = 0x86,     //Переход на какую то позицию
    JB              = 0x87,     //Условные переходы
    JBE             = 0x88,
    JA              = 0x89,
    JAE             = 0x8A,
    JE              = 0x8B,
    JNE             = 0x8C,

    RET             = 0x09,     //Возвращение

    //Коды аргументов(4 бита):
    NUM_ARG         = 0b0001,
    REG_ARG         = 0b0010,
    RAM_ARG         = 0b0100,
    ADR_ARG         = 0b1000,
    RAMREG_ARG      = 0b0110,
    REGNUM_ARG      = 0b0011,
};

enum GENERAL_ERRS
{
    FATAL_ERR = 404,
};

/*
Обработка аргументов:

push [rax] 0000 0110
push [rax + 5] 0000 0111        [rax\0+\05]
push [5] 0000 0101
push rax 0000 0100
push 5 0000 0001


mov [rax + 5],[bax + 1] 0101 1111 0101 0111
*/

#endif