#include "tokenization.h"

_INIT_LOG()

int take_token(token_t *token_ptr, int *pos, char *buff);

enum SYNTAX_CH syntax_detect(const char word);

enum MACHINE_CODE cmd_detect(const char *buff, int *pos);

void token_dump(token_arr *tokens);

int tokenize(token_arr *tokens, const char *file_name)
{
    assert(tokens);
    assert(file_name);

    _OPEN_LOG("logs/tokenization.log");

    char *buff      = NULL;
    int file_size   = 0;

    LOG("> reading file:\n");
    int error = file_read(&buff, &file_size, file_name);
    if (error)
    {
        LOG("[error]>>> error occured while reading a file\n");
        _CLOSE_LOG();
        return FILEREAD_ERR;
    }
    LOG("> file was read, starting tokenization\n");

    int token_arr_size = 50;
    int pos = 0;
    token_t *token_holder = (token_t *)calloc(token_arr_size, sizeof(token_t));
    if (!token_holder)
    {
        LOG("[error]>>> token array memory allocation error\n");
        free(buff);
        _CLOSE_LOG();
        return TOKEN_MEM_ALC_ERR;
    }
    
    int token_num = 0;
    for (;; token_num++)
    {
        if (pos == file_size)
            break;

        if (token_num == token_arr_size)
        {
            token_arr_size += 50;
            token_t *temp_tok_arr = (token_t *)realloc(token_holder, token_arr_size * sizeof(token_t));
            if (!temp_tok_arr)
            {
                LOG("[error]>>> token array memory reallocation error\n");
                free(buff);
                free(token_holder);
                _CLOSE_LOG();
                return TOKEN_MEM_RLC_ERR;
            }
            token_holder = temp_tok_arr;
        }

        if (take_token(token_holder + token_num, &pos, buff))
            break;
    }

    tokens->tokens      = token_holder;
    tokens->token_num   = token_num;

    token_dump(tokens);

    free(buff);
    _CLOSE_LOG();
    return 0;
}

int take_token(token_t *token_ptr, int *pos, char *buff)
{
    assert(token_ptr);
    assert(pos);
    assert(buff);

    _SKIP_SPACES();

    if (buff[*pos] == ';')
    {
        LOG("> comment was found, skipping string:\n");
        while (buff[*pos] != '\n' && buff[*pos] != '\0')
            (*pos)++;

        if (buff[*pos] == '\0')
        {
            LOG("> end of file, tokenization done");
            return 1;
        }

        _POS_SHIFT(1);
        _SKIP_SPACES();
    }

    unsigned char code = 0;

    code = syntax_detect(buff[*pos]);
    if (code != NO_SYNT)
    {
        LOG("> syntax character \"%c\" detected\n", buff[*pos]);
        token_ptr->data.code    = code;
        token_ptr->type         = SYNTAX;

        _POS_SHIFT(1);
        return 0;
    }

    code = cmd_detect(buff, pos);
    if (code != NOCMD)
    {
        LOG("> command %#04X detected\n", buff[*pos]);
        token_ptr->data.code    = code;
        token_ptr->type         = CMD;

        return 0;
    }
    
    int n = 0;
    char reg = 0;
    sscanf(buff + *pos, "%cx %n", &reg, &n);
    if (n)
    {
        LOG("> register %cx was found\n", reg);
        token_ptr->data.code    = reg - 'a';
        token_ptr->type         = REGISTER;

        _POS_SHIFT(n);
        return 0;
    }

    data_t num = 0;
    sscanf(buff + *pos, DATA_NSPEC, &num, &n);
    if (n)
    {
        LOG(DATA_SPEC, num);
        LOG(" number was found\n");
        token_ptr->data.number  = num;
        token_ptr->type         = NUMBER;

        _POS_SHIFT(n);
        return 0;
    }

    char string[50] = {0};
    sscanf(buff + *pos, "%[A-Za-z:_]%n", string, &n);
    if (n)
    {
        LOG("> a word \"%s\" was found\n", string);
        memcpy(&(token_ptr->data.string), string, 50 * sizeof(char));
        token_ptr->type         = STRING;

        _POS_SHIFT(n);
        return 0;
    }

    LOG("> no lexemma's was found, returning 1\n");
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
            _POS_SHIFT(commands[i].cmd_len);
            return commands[i].cmd_num;
        }
    
    return NOCMD;
}

void token_dump(token_arr *tokens)
{
#ifdef DEBUG
    assert(tokens);

    LOG(">>------------------starting token dump:---------------\n");
    for (int i = 0; i < tokens->token_num; i++)
    {
        LOG("%d)\n", i);
        switch (tokens->tokens[i].type)
        {
        case CMD:
            LOG("\tDATA: %#04X\n", tokens->tokens[i].data.code);
            LOG("\tTYPE: COMMMAND\n");
            break;
        
        case SYNTAX:
            LOG("\tDATA: %d\n", tokens->tokens[i].data.code);
            LOG("\tTYPE: SYNTAX CHARACTER\n");
            break;

        case REGISTER:
            LOG("\tDATA: %cx\n", tokens->tokens[i].data.code + 'a');
            LOG("\tTYPE: REGISTER\n");
            break;

        case NUMBER:
            LOG("\tDATA: ");
            LOG(DATA_SPEC, tokens->tokens[i].data.number);
            LOG("\n\tTYPE: NUMBER\n");
            break;

        case STRING:
            LOG("\tDATA: %s\n", tokens->tokens[i].data.string);
            LOG("\tTYPE: STRING\n");
            break;

        default:
            LOG("[error]>>> undefined token type\n");
            break;
        }
    }

    LOG(">>--------------------token dump ended---------------\n");
#endif
    return;
}