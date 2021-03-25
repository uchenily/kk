#include "common.h"
#include "scanner.h"
#include "string.h"

// define global variable
Scanner scanner;

void initScanner(const char * source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static bool isAtEnd() {
    return *scanner.current == '\0';
}

static char advance() {
    return *scanner.current++;
}

static bool match(char expected) {
    if(isAtEnd()) return false;
    if(*scanner.current != expected) return false;

    scanner.current++;
    return true;
}

// TODO: error handling
static char peek(int index) {
    return scanner.current[index];
}

static void skipCharacters() {
    for(;;) {
        char ch = peek(0);
        switch(ch) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '#':
                while(peek(0) != '\n' && !isAtEnd())
                    advance();
                break;
            default:
                return;
        }
    }
}

static bool isDigit(char ch) {
    return ch >= '0' && ch <= '9';
}

static bool isAlpha(char ch) {
    if(ch == '_') return true;
    return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
}

static TokenType checkKeyword(int length, const char * keyword,
        TokenType type) {
    if(scanner.current - scanner.start == length &&
        memcmp(scanner.start, keyword, length) == 0) return type;

    return TOKEN_IDENTIFIER;
}

static TokenType identifierType() {
    switch(*scanner.start) {
        case 'a': return checkKeyword(3, "and",    TOKEN_AND);
        case 'c': return checkKeyword(5, "class",  TOKEN_CLASS);
        case 'd': return checkKeyword(3, "def",    TOKEN_DEF);
        case 'e': return checkKeyword(4, "else",   TOKEN_ELSE);
        case 'f': return checkKeyword(3, "for",    TOKEN_FOR);
        case 'i': return checkKeyword(2, "if",     TOKEN_IF);
        case 'o': return checkKeyword(2, "or",     TOKEN_OR);
        case 'r': return checkKeyword(6, "return", TOKEN_RETURN);
        case 's': return checkKeyword(4, "self",   TOKEN_SELF);
        case 'v': return checkKeyword(3, "var",    TOKEN_VAR);
        case 'w': return checkKeyword(5, "while",  TOKEN_WHILE);
    }
    return TOKEN_IDENTIFIER;
}

static Token makeToken(TokenType type) {
    // string
    if(type == TOKEN_STRING) {
        while(peek(0) != '"' && !isAtEnd()) {
            if(peek(0) == '\n') scanner.line++;
            advance();
        }

        if(isAtEnd()) return makeToken(TOKEN_ERROR);

        // the closing quote.
        advance();
    }

    // number
    if(type == TOKEN_NUMBER) {
        while(isDigit(peek(0))) advance();

        // looking for a fractional part
        if(peek(0) == '.' && isDigit(peek(1))) {
            advance();
            while(isDigit(peek(0))) advance();
        }
    }

    // identifier or keyword
    if(type == TOKEN_IDENTIFIER) {
        while((isAlpha(peek(0)) || isDigit(peek(0)))
                && !isAtEnd())
            advance();

        TokenType type = identifierType();
        if(type != TOKEN_IDENTIFIER)
            return makeToken(type);
    }

    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = scanner.current - scanner.start;
    token.line = scanner.line;
    return token;
}

Token scanToken() {
    skipCharacters();
    scanner.start = scanner.current;

    if(isAtEnd()){
        Token eof = makeToken(TOKEN_EOF);
        return eof;
    }

    char ch = advance();
    switch(ch) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': return makeToken(TOKEN_NUMBER);

        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z': return makeToken(TOKEN_IDENTIFIER);

        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);

        case '!': return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=': return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<': return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

        case '"': return makeToken(TOKEN_STRING);
    }

    Token error = makeToken(TOKEN_ERROR);
    return error;
}
