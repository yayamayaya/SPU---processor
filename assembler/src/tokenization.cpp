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

struct Lexer_t
{
    Stack<token_t> *tokens;
    char *buff;
    line_t line_counter;
    size_t pos;
};

int take_token(Lexer_t *lex);

void skip_spaces(Lexer_t *lex);

bool is_EOF(Lexer_t *lex);

bool skipped_comment(Lexer_t *lex);

bool take_syntax(Lexer_t *lex);

enum SYNTAX_CH syntax_detect(const char word);

bool take_cmd(Lexer_t *lex);

enum CommandCodes cmd_detect(Lexer_t *lex);

bool take_register(Lexer_t *lex);

bool take_number(Lexer_t *lex);

bool take_string(Lexer_t *lex);

void token_dump(Stack<token_t> *tokens);


int tokenize(Stack<token_t> *tokens)
{
    assert(tokens);

    Lexer_t lexer = {tokens, NULL, 1, 0};

    LOG("reading file:");
    int error = file_read(&(lexer.buff), NULL, input_file_name());
    if (error)
    {
        LOG_ERR("Error occured while reading a file");
        return FILEREAD_ERR;
    }
    LOG("file was read, starting tokenization");
    
    int ret_val = 0;
    while (!ret_val)
        ret_val = take_token(&lexer);

    ret_val = ret_val == TOKENIZATION_OK ? 0 : ret_val;

    if (tokens_dump_enabled())
        token_dump(tokens);

    free(lexer.buff);
    return ret_val;
}

#define TRY_TAKE(detector, ret_val) \
    if (detector)                   \
        return ret_val

int take_token(Lexer_t *lex)
{
    assert(lex);

    skip_spaces(lex);

    TRY_TAKE(is_EOF(lex), TOKENIZATION_OK);
    TRY_TAKE(skipped_comment(lex), 0);
    TRY_TAKE(take_syntax(lex), 0);
    TRY_TAKE(take_cmd(lex), 0);
    TRY_TAKE(take_register(lex), 0);
    TRY_TAKE(take_number(lex), 0);
    TRY_TAKE(take_string(lex), 0);

    LOG_ERR("no lexemma's was found, compilation error");
    fprintf(stderr, "Compilation error at line %lu: unknown token\n", lex->line_counter);
    return TOKENIZATION_BAD;
}

#undef TRY_TAKE

void skip_spaces(Lexer_t *lex)
{
    char *buff = lex->buff;

    while (isspace(lex->buff[lex->pos]))
    {
        if (buff[lex->pos] == '\n')
            lex->line_counter++;

        (lex->pos)++; 
    }        
    LOG("spaces skipped, current position is: %d", lex->pos);
}

bool is_EOF(Lexer_t *lex)
{
    return lex->buff[lex->pos] == '\0';
}

bool skipped_comment(Lexer_t *lex)
{
    char *buff = lex->buff;

    if (buff[lex->pos] == ';')
    {
        LOG("comment was found, skipping string");
        while (buff[lex->pos] != '\n' && !is_EOF(lex))
            (lex->pos)++;

        return true;
    }

    return false;
}

bool take_syntax(Lexer_t *lex)
{
    Stack<token_t> *tokens = lex->tokens;

    byte_t code = syntax_detect(lex->buff[lex->pos]);
    if (code != NO_SYNT)
    {
        LOG("syntax character '%c' detected", lex->buff[lex->pos]);
        tokens->push({lex->line_counter, {.code = code}, SYNTAX});

        lex->pos += 1;
        return true;
    }

    return false;
}

enum SYNTAX_CH syntax_detect(const char word)
{
    for (int i = 0; i < SYNTAX_CH_NUMBER; i++)
        if (syntax_chars[i].syntax_char == word)
            return syntax_chars[i].char_code;

    return NO_SYNT;
}

bool take_cmd(Lexer_t *lex)
{
    Stack<token_t> *tokens = lex->tokens;

    byte_t code = cmd_detect(lex);
    if (code != NO_CMD)
    {
        LOG("command '%#04X' detected", code);
        tokens->push({lex->line_counter, {.code = code}, CMD});

        return true;
    }

    return false;
}

enum CommandCodes cmd_detect(Lexer_t *lex)
{
    char *buff = lex->buff;

    for (int i = 0; i < COMMANDS_NUM; i++)
        if (strncmp(commands[i].cmd_name, buff + lex->pos, commands[i].cmd_len) == 0)
        {
            (lex->pos) += commands[i].cmd_len;
            return commands[i].cmd_num;
        }

    return NO_CMD;
}

bool take_register(Lexer_t *lex)
{
    char *buff = lex->buff;
    Stack<token_t> *tokens = lex->tokens;

    int n = 0;
    char reg = 0;
    sscanf(buff + lex->pos, "%cx%n", &reg, &n);
    if (n)
    {
        LOG("register '%cx' was found", reg);
        unsigned char register_number = (byte_t)(reg - 'a');
        tokens->push({lex->line_counter, {.code = register_number}, REGISTER});

        lex->pos += (size_t)n;
        return true;
    }

    return false;
}

bool take_number(Lexer_t *lex)
{
    char *buff = lex->buff;
    Stack<token_t> *tokens = lex->tokens;

    data_t num = 0;
    int n = 0;
    sscanf(buff + lex->pos, DATA_NSPEC, &num, &n);
    if (n)
    {
        LOG("found number", DATA_SPEC, num);
        tokens->push({lex->line_counter, {.number = num}, NUMBER});

        lex->pos += (size_t)n;
        return true;
    }

    return false;
}

bool take_string(Lexer_t *lex)
{
    char *buff = lex->buff;
    Stack<token_t> *tokens = lex->tokens;

    token_t name_token = {};
    int n = 0;
    sscanf(buff + lex->pos, "%[A-Za-z:./_0-9]%n", name_token.data.string, &n);
    if (n)
    {
        LOG("a possible label '%s' was found", name_token.data.string);
        name_token.line = lex->line_counter;
        name_token.type = STRING;

        tokens->push(name_token);

        lex->pos += (size_t)n;
        return true;
    }

    return false;
}

void token_dump(Stack<token_t> *tokens)
{
    assert(tokens);

    printf("------------------Starting token dump--------------------\n");
    for (size_t i = 0; i < tokens->stack_size(); i++)
    {
        token_t token = *tokens->at(i);

        printf("%3lu) ", i);
        switch (token.type)
        {
        case CMD:
            printf("%#04x [COMMAND]\n", token.data.code);
            break;

        case SYNTAX:
            printf("%d [SYNTAX CHAR]\n", token.data.code);
            break;

        case REGISTER:
            printf("%cx [REGISTER]\n", token.data.code + 'a');
            break;

        case NUMBER:
            printf(DATA_SPEC " [NUMBER]\n", token.data.number);
            break;

        case STRING:
            printf("'%s' [STRING]\n", token.data.string);
            break;

        default:
            LOG_ERR("undefined token type");
            fprintf(stderr, "Compiler error: undefined token type\n");
            break;
        }
    }

    printf("---------------------------------------------------------\n");
    return;
}
