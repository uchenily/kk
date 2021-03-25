#include "debug.h"
#include "chunk.h"
#include "scanner.h"

void disassembleChunk(Chunk * chunk, const char * name) {
    printf("=== %s ===\n", name);
    printf("Offset  LineNo  OpCode(s)\n");
    printf("-------------------------\n");

    for(int offset = 0; offset < chunk->count; ) {
        offset = disassembleInstruction(chunk, offset);
    }
    printf("-------------------------\n");
}

int disassembleInstruction(Chunk * chunk, int offset) {
    printf("%04d     ", offset);
    if(offset == 0 ||
            offset > 0 && chunk->lines[offset] != chunk->lines[offset - 1]) {
        printf("%4dl  ", chunk->lines[offset]);
    } else {
        printf("%7s", "");
    }

    byte_t instruction = chunk->codes[offset];
    switch(instruction) {
        case OP_CONSTANT: {
            byte_t index = chunk->codes[offset + 1]; // get index of constants pool
            byte_t constant = chunk->constants.values[index]; // get constant value
            printf("%-16s", "OP_CONSTANT");
            printValue(constant);
            printf("\n");
            return offset + 2;
        }
        case OP_NEGATE: {
            printf("OP_NEGATE\n");
            return offset + 1;
        }
        case OP_ADD: {
            printf("OP_ADD\n");
            return offset + 1;
        }
        case OP_SUBSTRACT: {
            printf("OP_SUBSTRACT\n");
            return offset + 1;
        }
        case OP_MULTIPLY: {
            printf("OP_MULTIPLY\n");
            return offset + 1;
        }
        case OP_DIVIDE: {
            printf("OP_DIVIDE\n");
            return offset + 1;
        }
        case OP_RETURN: {
            printf("OP_RETURN\n");
            return offset + 1;
        }
        default: {
            printf("Unknown opcode: 0x%02x\n", instruction);
            return offset + 1;
        }
    }
}

void printStack(VM * vm) {
    for(KkValue * slot = vm->stack; slot < vm->stackTop; slot++) {
        printf("[ ");
        printValue(*slot);
        printf(" ]");
    }
    printf("\n");
}

const char * tokenType(TokenType type) {
    switch(type) {
        case TOKEN_LEFT_PAREN:          return "TOKEN_LEFT_PAREN";
        case TOKEN_RIGHT_PAREN:         return "TOKEN_RIGHT_PAREN";
        case TOKEN_LEFT_BRACE:          return "TOKEN_LEFT_BRACE";
        case TOKEN_RIGHT_BRACE:         return "TOKEN_RIGHT_BRACE";
        case TOKEN_COMMA:               return "TOKEN_COMMA";
        case TOKEN_DOT:                 return "TOKEN_DOT";
        case TOKEN_MINUS:               return "TOKEN_MINUS";
        case TOKEN_PLUS:                return "TOKEN_PLUS";
        case TOKEN_SEMICOLON:           return "TOKEN_SEMICOLON";
        case TOKEN_SLASH:               return "TOKEN_SLASH";
        case TOKEN_STAR:                return "TOKEN_STAR";
        case TOKEN_BANG:                return "TOKEN_BANG";
        case TOKEN_BANG_EQUAL:          return "TOKEN_BANG_EQUAL";
        case TOKEN_EQUAL:               return "TOKEN_EQUAL";
        case TOKEN_EQUAL_EQUAL:         return "TOKEN_EQUAL_EQUAL";
        case TOKEN_GREATER:             return "TOKEN_GREATER";
        case TOKEN_GREATER_EQUAL:       return "TOKEN_GREATER_EQUAL";
        case TOKEN_LESS:                return "TOKEN_LESS";
        case TOKEN_LESS_EQUAL:          return "TOKEN_LESS_EQUAL";
        case TOKEN_IDENTIFIER:          return "TOKEN_IDENTIFIER";
        case TOKEN_STRING:              return "TOKEN_STRING";
        case TOKEN_NUMBER:              return "TOKEN_NUMBER";
        case TOKEN_AND:                 return "TOKEN_AND";
        case TOKEN_CLASS:               return "TOKEN_CLASS";
        case TOKEN_ELSE:                return "TOKEN_ELSE";
        case TOKEN_FOR:                 return "TOKEN_FOR";
        case TOKEN_DEF:                 return "TOKEN_DEF";
        case TOKEN_IF:                  return "TOKEN_IF";
        case TOKEN_OR:                  return "TOKEN_OR";
        case TOKEN_RETURN:              return "TOKEN_RETURN";
        case TOKEN_SELF:                return "TOKEN_SELF";
        case TOKEN_VAR:                 return "TOKEN_VAR";
        case TOKEN_WHILE:               return "TOKEN_WHILE";
        case TOKEN_ERROR:               return "TOKEN_ERROR";
        case TOKEN_EOF:                 return "TOKEN_EOF";
    }
}

void printTokens(const char * source) {
    // we need reinitialize scanner after debugging
    initScanner(source);
    printf("%4s %-20s %s\n", "LINE", "TYPE", "VALUE");
    printf("-------------------------------\n");
    for(;;) {
        Token token = scanToken();

        printf("%4d %-20s %.*s\n", token.line, tokenType(token.type),
                token.length, token.start);
        if(token.type == TOKEN_EOF) break;
    }
    printf("-------------------------------\n");
}
