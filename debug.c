#include "debug.h"
#include "chunk.h"

void disassembleChunk(Chunk * chunk, const char * name) {
    printf("=== %s ===\n", name);

    for(int offset = 0; offset < chunk->count; ) {
        offset = disassembleInstruction(chunk, offset);
    }
}

int disassembleInstruction(Chunk * chunk, int offset) {
    printf("%04d ", offset);

    byte_t instruction = chunk->codes[offset];
    byte_t code;
    switch(instruction) {
        case OP_CONSTANT:
            code = chunk->codes[offset + 1]; // get index of constants pool
            code = chunk->constants.values[code]; // get constant value
            printf("%-16s", "OP_CONSTANT");
            printValue(code);
            printf("\n");
            return offset + 2;
        case OP_RETURN:
            printf("OP_RETURN\n");
            return offset + 1;
        default:
            printf("Unknown opcode: 0x%02x\n", instruction);
            return offset + 1;
    }
}
