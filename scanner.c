#include "common.h"
#include "scanner.h"
#include "string.h"

void initScanner(Scanner * scanner, const char * source) {
    scanner->start = source;
    scanner->current = source;
    scanner->line = 1;
}

bool isAtEnd(Scanner * scanner) {
    return *scanner->current == '\0';
}

char advance(Scanner * scanner) {
    return *scanner->current++;
}

bool match(Scanner * scanner, char expected) {
    if(isAtEnd(scanner)) return false;
    if(*scanner->current != expected) return false;

    scanner->current++;
    return true;
}

// TODO: error handling
char peek(Scanner * scanner, int index) {
    return scanner->current[index];
}

void skipCharacters(Scanner * scanner) {
    for(;;) {
        char ch = peek(scanner, 0);
        switch(ch) {
            case ' ':
            case '\r':
            case '\t':
                advance(scanner);
                break;
            case '\n':
                scanner->line++;
                advance(scanner);
                break;
            case '#':
                while(peek(scanner, 0) != '\n' && !isAtEnd(scanner))
                    advance(scanner);
                break;
            default:
                return;
        }
    }
}

bool isDigit(char ch) {
    return ch >= '0' && ch <= '9';
}

bool isAlpha(char ch) {
    if(ch == '_') return true;
    return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
}

TokenType checkKeyword(Scanner * scanner, int length, const char * keyword,
        TokenType type) {
    if(scanner->current - scanner->start == length &&
        memcmp(scanner->start, keyword, length) == 0) return type;

    return TOKEN_IDENTIFIER;
}

TokenType identifierType(Scanner * scanner) {
    switch(*scanner->start) {
        case 'a': return checkKeyword(scanner, 3, "and",    TOKEN_AND);
        case 'c': return checkKeyword(scanner, 5, "class",  TOKEN_CLASS);
        case 'd': return checkKeyword(scanner, 3, "def",    TOKEN_DEF);
        case 'e': return checkKeyword(scanner, 4, "else",   TOKEN_ELSE);
        case 'f': return checkKeyword(scanner, 3, "for",    TOKEN_FOR);
        case 'i': return checkKeyword(scanner, 2, "if",     TOKEN_IF);
        case 'o': return checkKeyword(scanner, 2, "or",     TOKEN_OR);
        case 'r': return checkKeyword(scanner, 6, "return", TOKEN_RETURN);
        case 's': return checkKeyword(scanner, 4, "self",   TOKEN_SELF);
        case 'v': return checkKeyword(scanner, 3, "var",    TOKEN_VAR);
        case 'w': return checkKeyword(scanner, 5, "while",  TOKEN_WHILE);
    }
    return TOKEN_IDENTIFIER;
}

Token makeToken(Scanner * scanner, TokenType type) {
    // string
    if(type == TOKEN_STRING) {
        while(peek(scanner, 0) != '"' && !isAtEnd(scanner)) {
            if(peek(scanner, 0) == '\n') scanner->line++;
            advance(scanner);
        }

        if(isAtEnd(scanner)) return makeToken(scanner, TOKEN_ERROR);

        // the closing quote.
        advance(scanner);
    }

    // number
    if(type == TOKEN_NUMBER) {
        while(isDigit(peek(scanner, 0))) advance(scanner);

        // looking for a fractional part
        if(peek(scanner, 0) == '.' && isDigit(peek(scanner, 1))) {
            advance(scanner);
            while(isDigit(peek(scanner, 0))) advance(scanner);
        }
    }

    // identifier or keyword
    if(type == TOKEN_IDENTIFIER) {
        while((isAlpha(peek(scanner, 0)) || isDigit(peek(scanner, 0)))
                && !isAtEnd(scanner))
            advance(scanner);

        TokenType type = identifierType(scanner);
        if(type != TOKEN_IDENTIFIER)
            return makeToken(scanner, type);
    }

    Token token;
    token.type = type;
    token.start = scanner->start;
    token.length = scanner->current - scanner->start;
    token.line = scanner->line;
    return token;
}

Token scanToken(Scanner * scanner) {
    skipCharacters(scanner);
    scanner->start = scanner->current;

    if(isAtEnd(scanner)){
        Token eof = makeToken(scanner, TOKEN_EOF);
        return eof;
    }

    char ch = advance(scanner);
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
        case '9': return makeToken(scanner, TOKEN_NUMBER);

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
        case 'Z': return makeToken(scanner, TOKEN_IDENTIFIER);

        case '(': return makeToken(scanner, TOKEN_LEFT_PAREN);
        case ')': return makeToken(scanner, TOKEN_RIGHT_PAREN);
        case '{': return makeToken(scanner, TOKEN_LEFT_BRACE);
        case '}': return makeToken(scanner, TOKEN_RIGHT_BRACE);
        case ';': return makeToken(scanner, TOKEN_SEMICOLON);
        case ',': return makeToken(scanner, TOKEN_COMMA);
        case '.': return makeToken(scanner, TOKEN_DOT);
        case '-': return makeToken(scanner, TOKEN_MINUS);
        case '+': return makeToken(scanner, TOKEN_PLUS);
        case '/': return makeToken(scanner, TOKEN_SLASH);
        case '*': return makeToken(scanner, TOKEN_STAR);

        case '!': return makeToken(scanner, match(scanner, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=': return makeToken(scanner, match(scanner, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<': return makeToken(scanner, match(scanner, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return makeToken(scanner, match(scanner, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

        case '"': return makeToken(scanner, TOKEN_STRING);
    }

    Token error = makeToken(scanner, TOKEN_ERROR);
    return error;
}
