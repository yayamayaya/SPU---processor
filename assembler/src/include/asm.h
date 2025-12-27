#ifndef ASM_H
#define ASM_H

typedef unsigned char byte_t;

typedef double data_t;
#define DATA_NSPEC      "%lg%n"
#define DATA_SPEC       "%lg"
#define DATA_PRINT_SPEC "%.02lf\n"

/* New command architecture: 
 * 0b[000]{00000} ([00]{000}{000})
 * [] - argument commands bits: 
 * 000 - no argument
 * 100 - one or two arguments
 * 001 - special argument (jump command group)
 *
 * {} - command itself
 * 
 * ([]) - argument number bits:
 *  01  - there's one argument in lower 3 bits
 *  11  - there's two arguments in leftofer 6 bits
 * 
 * ({}) - argument type bits:
 *  001 - number argument                   - push 1
 *  010 - register argument                 - push cx
 *  101 - ram + number argument             - push [15]
 *  110 - ram + register argument           - push [dx]
 *  111 - ram + register + number argument  - push [bx + 1]
 */

enum CommandCodes
{
// No argument commands
    ADD             = 0b00000001,     // Addition
    SUB             = 0b00000010,     // Substraction
    MULT            = 0b00000011,     // Multiplication
    DIV             = 0b00000100,     // Divison
    SQRT            = 0b00000101,     // Square root
    POW             = 0b00000110,     // Power
 
    LOG_E           = 0b00001000,     // Equal
    LOG_NE          = 0b00001001,     // Not equal
    LOG_A           = 0b00001010,     // Above
    LOG_AE          = 0b00001011,     // Above or equal
    LOG_B           = 0b00001100,     // Below
    LOG_BE          = 0b00001101,     // Below or equal
 
    IN              = 0b00010000,     // Input from stdin   
    OUT             = 0b00010001,     // Output current stack top to stdout

    DISPLAY         = 0b00011001,     // Display current RAM
 
    RET             = 0b00011110,     // Return to the calling function
    HALT            = 0b00011111,     // Finish the program

// One or two argument commands
    PUSH_GRP        = 0b10000001,     // Push to stack
    POP_GRP         = 0b10000010,     // Pop from stack
     
    MOV_GRP         = 0b10000100,     // Move from one place to another


// Integer argument commands
    CALL            = 0b00110000,     // 'Function' call

    JMP             = 0b00101111,     // Jump to label
    JB              = 0b00101000,     // LOG_B  jump
    JBE             = 0b00101001,     // LOG_BE jump
    JA              = 0b00101010,     // LOG_A  jump
    JAE             = 0b00101011,     // LOG_AE jump
    JE              = 0b00101100,     // LOG_E  jump
    JNE             = 0b00101101,     // LOG_NE jump

    WAIT            = 0b00111111,     // Pause program for x seconds

// Graphical work commands
    LOAD            = 0b01100001,     // Load all file contents to RAM, starting !!from!! 0

    NO_CMD          = 0b11100000,     // No command
};

enum ArgumentCommandType
{
    ARG_TYPE_MASK   = 0b11100000,     // A mask for argument type

    NO_ARGUMENT_CMD = 0b00000000,     // Command with no argument
    ARGUMENT_CMD    = 0b10000000,     // Command with 1 or 2 arguments
    SPEC_ARG_CMD    = 0b00100000,     // Command with int argument
    GRAPHICAL_CMD   = 0b01100000,     // Command with for graphical generation
};

enum ArgumentKind
{
    NUMBER_TYPE     = 0b001,          // Number
    REGISTER_TYPE   = 0b010,          // Register
    RAM_TYPE        = 0b100,          // Ram type
    RAMNUM_TYPE     = 0b101,          // Ram + number
    RAMREG_TYPE     = 0b110,          // Ram + register
    RAMREGNUM_TYPE  = 0b111,          // Ram + register + number
};

enum ArgumentNumber
{
    ONE_ARG         = 0b01000000,      // Only one argument
    TWO_ARGS        = 0b11000000,      // Two arguments
};

#endif