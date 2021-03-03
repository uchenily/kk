#ifndef KK_SCANNER_H
#define KK_SCANNER_H

typedef struct {
    const char * start;
    const char * current;
    int          line;
} Scanner;

typedef enum {
    /* single-character tokens */
    TOKEN_LEFT_PAREN,       // (
    TOKEN_RIGHT_PAREN,      // )
    TOKEN_LEFT_BRACE,       // {
    TOKEN_RIGHT_BRACE,      // }
    TOKEN_COMMA,            // ,
    TOKEN_DOT,              // .
    TOKEN_MINUS,            // -
    TOKEN_PLUS,             // +
    TOKEN_SEMICOLON,        // ;
    TOKEN_SLASH,            // /
    TOKEN_STAR,             // *

    /* one or two character tokens */
    TOKEN_BANG,             // !
    TOKEN_BANG_EQUAL,       // !=
    TOKEN_EQUAL,            // =
    TOKEN_EQUAL_EQUAL,      // ==
    TOKEN_GREATER,          // >
    TOKEN_GREATER_EQUAL,    // >=
    TOKEN_LESS,             // <
    TOKEN_LESS_EQUAL,       // <=

    /* literals */
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_NUMBER,

    /* keywords */
    TOKEN_AND,              // and
    TOKEN_CLASS,            // class
    TOKEN_ELSE,             // else
    TOKEN_FOR,              // for
    TOKEN_DEF,              // def
    TOKEN_IF,               // if
    //TOKEN_NIL,              // nil
    TOKEN_OR,               // or
    TOKEN_RETURN,           // return
    TOKEN_SELF,             // self
    TOKEN_VAR,              // var
    TOKEN_WHILE,            // while

    /* others */
    TOKEN_ERROR,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType    type;
    const char * start;
    int          length;
    int          line;
} Token;

void initScanner(Scanner * scanner, const char * source);
Token scanToken(Scanner * scanner);

#endif /* KK_SCANNER_H */
