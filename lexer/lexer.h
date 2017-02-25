#ifndef C_LEXER_H
#define C_LEXER_H

#include <stdio.h>

#define error(msg, ...)		fprintf(stderr, "\033[1;41merror:\033[0m " msg, ##__VA_ARGS__)
#define warning(msg, ...)	fprintf(stderr, "\033[1;43mwarning:\033[0m " msg, ##__VA_ARGS__)
#define info(msg, ...)		fprintf(stdout, "\033[1;44minfo:\033[0m " msg, ##__VA_ARGS__)
#define debug(msg, ...)     fprintf(stdout, "\033[1;46mdebug:\033[0m " msg, ##__VA_ARGS__)
#define exit_msg(status) fprintf(stdout, "\033[1;46mexit:\033[0m from \033[1;33m%s\033[0m with code %d\n", __FUNCTION__, (int)status) 

typedef struct _lexer_out_t lexer_out_t;
struct _lexer_out_t{
    size_t tokc;
    lexer_tok_t *tokv;
};

typedef struct _lexer_in_t lexer_in_t;
struct _lexer_in_t{
    size_t size;
    char *name;
    char *content;
};
/* types for tokens */

#define T_ENDOFAPP 1
#define T_BLOCK_OPEN 2
#define T_BLOCK_CLOSE 3 
#define T_BR_OPEN 4
#define T_BR_CLOSE 5
#define T_INDEX_OPEN 6
#define T_INDEX_CLOSE 7
#define T_SEPARATOR 8  
#define T_COMMA 9
#define T_KEYWORD 10
#define T_INT_LITERAL 11
#define T_FLOAT_LITERAL 12
#define T_STR_LITERAL 13
#define T_CHAR_LITERAL 14
#define T_IDENTIFIER 15
#define T_ILLEGAL

typedef struct _lexer_tok_t lexer_tok_t;
struct _lexer_tok_t{
    char type;
    char *content;
};

#define IS_LEGAL_ID_CHAR(expr, first) (                                \
	((expr) >= 'a' && (expr) <= 'z' ) ||                               \
	((expr) >= 'A' && (expr) <= 'Z') ||                                \
	(isdigit(expr) && !isdigit(first)) ||                              \
	((expr)=='$' || (expr)=='+' || (expr)=='-' || (expr)=='_')         \
	)

extern int lexer(lexer_in_t *input, lexer_out_t *output);
extern void lexer_output_create(lexer_out_t *output);

#endif