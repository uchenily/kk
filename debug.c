#include "debug.h"
#include "chunk.h"

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
