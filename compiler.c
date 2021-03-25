#include "common.h"
#include "compiler.h"
#include "scanner.h"
#include "parser.h"

#if defined(DEBUG_PRINT_TOKEN) || defined(DEBUG_PRINT_BYTE_CODE)
#include "debug.h"
#endif /* defined(DEBUG_PRINT_TOKEN) || defined(DEBUG_PRINT_BYTE_CODE) */

/* use global variables of struct Scanner, Parser, Chunk ...
 * so we can avoid pass these pointers from one function to another */

// use global variable, not define it.
Scanner scanner;
Parser parser;

// define global variable, use static to limit scope.
static Chunk * compilingChunk;
#define MESSAGE_MAX 30
static char message[MESSAGE_MAX + 1];

// avoid using long messages
static void generateMessage(const char * errorMessage) {
    const char * p = errorMessage;
    int i = 0;
    while(*p != '\0' && *p != '\n' && i < MESSAGE_MAX) {
        message[i++] = *p++;
    }
    message[i] = '\0';
}

static void errorAt(Token * token, const char * errorMessage) {
    if(parser.hadError) return;
    parser.hadError = true;

    if(token->type == TOKEN_EOF) {
        fprintf(stderr, "[line %d] Error at end", token->line);
    } else if(token->type == TOKEN_ERROR) {
        fprintf(stderr, "[line %d] Error", token->line);
    } else {
        fprintf(stderr, "[line %d] Error at '%.*s'", token->line,
                token->length, token->start);
    }

    generateMessage(errorMessage);
    fprintf(stderr, ": %s\n", message);
}

static void errorAtCurrent(const char * errorMessage) {
    errorAt(&parser.current, errorMessage);
}

static void errorAtPrevious(const char * errorMessage) {
    errorAt(&parser.previous, errorMessage);
}

static void advance() {
    parser.previous = parser.current;

    parser.current = scanToken();
    if(parser.current.type == TOKEN_ERROR) {
        errorAtCurrent(parser.current.start);
    }
}

static void consume(TokenType expectType, const char * errorMessage) {
    if(parser.current.type != expectType) {
        errorAtCurrent(errorMessage);
    } else {
        advance();
    }
}

static void emitByte(byte_t byte) {
    writeChunk(compilingChunk, byte, parser.previous.line);
}

static void endCompiler() {
    emitByte(OP_RETURN);
}

static void emit2Bytes(byte_t byte1, byte_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static byte_t makeConstant(KkValue value) {
    int constant = addConstant(compilingChunk, value);
    // TODO: handle constant >= 256
    return (byte_t)constant;
}

static void emitConstant(KkValue value) {
    emit2Bytes(OP_CONSTANT, makeConstant(value));
}

static ParseRule * getRule(TokenType type);

static void parsePrecedence(Precedence precedence) {
    advance();
    ParseFunc prefixRule = getRule(parser.previous.type)->prefix;
    if(prefixRule == NULL) {
        errorAtPrevious("Expect expression.");
        return;
    }

    prefixRule();

    while(precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFunc infixRule = getRule(parser.previous.type)->infix;
        if(infixRule == NULL) {
            errorAtPrevious("Expect expression.");
            return;
        }

        infixRule();
    }
}

static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}

static void number() {
    int value = atoi(parser.previous.start);
    emitConstant(value);
}

static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void unary() {
    TokenType operatorType = parser.previous.type;

    // compile the operand.
    parsePrecedence(PREC_UNARY);

    // emit the operator instruction.
    switch(operatorType) {
        case TOKEN_MINUS:   emitByte(OP_NEGATE); break;
        default:
            return; // unreachable.
    }
}

static void binary() {
    TokenType operatorType = parser.previous.type;

    // compile the right operand.
    ParseRule * rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    // emit the operator instruction.
    switch(operatorType) {
        case TOKEN_PLUS:    emitByte(OP_ADD); break;
        case TOKEN_MINUS:   emitByte(OP_SUBSTRACT); break;
        case TOKEN_STAR:    emitByte(OP_MULTIPLY); break;
        case TOKEN_SLASH:   emitByte(OP_DIVIDE); break;
        default:
            return; // unreachable.
    }
}

ParseRule rules[] = {
    [TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE},
    [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
    [TOKEN_PLUS]          = {NULL,     binary, PREC_TERM},
    [TOKEN_SEMICOLON]     = {NULL,     NULL,   PREC_NONE},
    [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
    [TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR},
    [TOKEN_BANG]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_BANG_EQUAL]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EQUAL_EQUAL]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_GREATER]       = {NULL,     NULL,   PREC_NONE},
    [TOKEN_GREATER_EQUAL] = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LESS]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LESS_EQUAL]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_IDENTIFIER]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_STRING]        = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
    [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
    [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};

static ParseRule * getRule(TokenType type) {
    return &rules[type];
}

bool compile(const char * source, Chunk * chunk) {
#ifdef DEBUG_PRINT_TOKEN
    printTokens(source);
#endif
    initScanner(source);

    // global variable
    compilingChunk = chunk;

    parser.hadError = false;

    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");
    endCompiler();
#ifdef DEBUG_PRINT_BYTE_CODE
    if(!parser.hadError) {
        disassembleChunk(compilingChunk, "op code");
    }
#endif

    return !parser.hadError;
}
