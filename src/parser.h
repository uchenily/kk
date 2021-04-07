#ifndef KK_PARSER_H
#define KK_PARSER_H

#include "common.h"
#include "scanner.h"

typedef struct {
    Token current;
    Token previous;
    bool  hadError;
} Parser;

// declare external variable, not define it
extern Parser parser;

typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT,    // =
    PREC_OR,            // or
    PREC_AND,           // and
    PREC_EQUALITY,      // == !=
    PREC_COMPARISON,    // < > <= >=
    PREC_TERM,          // + -
    PREC_FACTOR,        // * /
    PREC_UNARY,         // ! -
    PREC_CALL,          // . ()
    PREC_PRIMARY
} Precedence;

typedef void (* ParseFunc)();

typedef struct {
    ParseFunc  prefix;
    ParseFunc  infix;
    Precedence precedence;
} ParseRule;

#endif /* KK_PARSER_H */
