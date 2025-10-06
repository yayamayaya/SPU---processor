#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "commands.h"
#include "file_reading.h"
#include "tokenization.h"
#include "options_parsing.h"
#include "log.h"
#include "stack.h"
#include "asm_arrays.h"

int take_token(Stack<token_t> *tokens, char *buff);

enum SYNTAX_CH syntax_detect(const char word);

enum MACHINE_CODE cmd_detect(const char *buff, int *pos);

void token_dump(Stack<token_t> *tokens);

int tokenize(Stack<token_t> *tokens)
{
    assert(tokens);

    char *buff         = NULL;

    LOG("reading file:");
    int error = file_read(&buff, NULL, input_file_name());
    if (error)
    {
        LOG_ERR("Error occured while reading a file");
        return FILEREAD_ERR;
    }
    LOG("file was read, starting tokenization");

    
    while (!take_token(tokens, buff));

    if (tokens_dump_enabled())
        token_dump(tokens);

    free(buff);
    return 0;
}

int take_token(Stack<token_t> *tokens, char *buff)
{
    assert(tokens);
    assert(buff);

    static int pos = 0;
    static line_t line = 1;

    LOG("skipping spaces");
    while (isspace(buff[pos]))
    {
        if (buff[pos] == '\n')
        {
            LOG("buff[pos] = %d", buff[pos]);
            line++;
        }

        (pos)++; 
    }        
    LOG("spaces skipped, current position is: %d, current char is: '%c'", pos, buff[pos]);

    if (buff[pos] == '\0')
    {
        LOG("end of file, tokenization done");
        return 1;
    }

    if (buff[pos] == ';')
    {
        LOG("comment was found, skipping string:");
        while (buff[pos] != '\n' && buff[pos] != '\0')
            (pos)++;

        return 0;
    }

    unsigned char code = 0;

    code = syntax_detect(buff[pos]);
    if (code != NO_SYNT)
    {
        LOG("syntax character \"%c\" detected", buff[pos]);
        tokens->push({line, {.code = code}, SYNTAX});

        _POS_SHIFT(1);
        return 0;
    }

    code = cmd_detect(buff, &pos);
    if (code != NOCMD)
    {
        LOG("command %#04X detected", buff[pos]);
        tokens->push({line, {.code = code}, CMD});

        return 0;
    }

    int n = 0;
    char reg = 0;
    sscanf(buff + pos, "%cx%n", &reg, &n);
    if (n)
    {
        LOG("register %cx was found", reg);
        unsigned char register_number = (unsigned char)(reg - 'a');
        tokens->push({line, {.code = register_number}, REGISTER});

        _POS_SHIFT(n);
        return 0;
    }

    data_t num = 0;
    sscanf(buff + pos, DATA_NSPEC, &num, &n);
    if (n)
    {
        LOG("" DATA_SPEC " number was found", num);
        tokens->push({line, {.number = num}, NUMBER});

        _POS_SHIFT(n);
        return 0;
    }

    token_t name_token = {};
    sscanf(buff + pos, "%[A-Za-z:_0-9]%n", name_token.data.string, &n);
    if (n)
    {
        LOG("a word \"%s\" was found", name_token.data.string);
        name_token.line = line;
        name_token.type = STRING;

        tokens->push(name_token);

        _POS_SHIFT(n);
        return 0;
    }

    LOG_ERR("no lexemma's was found, compilation error");
    printf("Compilation error at line %lu: unexpected token\n", line);
    return 1;
}

enum SYNTAX_CH syntax_detect(const char word)
{
    for (int i = 0; i < SYNTAX_CH_NUMBER; i++)
        if (syntax_chars[i].syntax_char == word)
            return syntax_chars[i].char_code;

    return NO_SYNT;
}

enum MACHINE_CODE cmd_detect(const char *buff, int *pos)
{
    assert(buff);
    assert(pos);

    for (int i = 0; i < COMMANDS_NUM; i++)
        if (strncmp(commands[i].cmd_name, buff + *pos, commands[i].cmd_len) == 0)
        {
            (*pos) += commands[i].cmd_len;
            LOG("position shifted, current position is %d, current char is '%c'", *pos, buff[*pos]);
            return commands[i].cmd_num;
        }

    return NOCMD;
}

void token_dump(Stack<token_t> *tokens)
{
    assert(tokens);

    printf("------------------Starting token dump--------------------\n");
    for (int i = 0; i < tokens->get_stack_size(); i++)
    {
        token_t token = tokens->get_data_on_pos(i);

        printf("%3d) ", i);
        switch (token.type)
        {
        case CMD:
            printf("DATA: %#04x [COMMAND]\n", token.data.code);
            break;

        case SYNTAX:
            printf("DATA: %d [SYNTAX CHAR]\n", token.data.code);
            break;

        case REGISTER:
            printf("DATA: %cx [REGISTER]\n", token.data.code + 'a');
            break;

        case NUMBER:
            printf("DATA: " DATA_SPEC " [NUMBER]\n", token.data.number);
            break;

        case STRING:
            printf("DATA: '%s' [STRING]\n", token.data.string);
            break;

        default:
            LOG_ERR("undefined token type");
            printf("Compiler error: undefined token type\n");
            break;
        }
    }

    printf("--------------------Token dump ended---------------------\n");
    return;
}
