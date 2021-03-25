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

#endif /* KK_PARSER_H */
