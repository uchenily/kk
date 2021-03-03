#include "common.h"
#include "compiler.h"
#include "scanner.h"
#include "debug.h"

void compile(const char * source) {
    Scanner scanner;
    initScanner(&scanner, source);
    printf("%4s %-20s %s\n", "LINE", "TYPE", "VALUE");
    printf("-------------------------------\n");
    for(;;) {
        Token token = scanToken(&scanner);

        printf("%4d %-20s %.*s\n", token.line, tokenType(token.type), token.length, token.start);
        if(token.type == TOKEN_EOF) break;
    }
}
