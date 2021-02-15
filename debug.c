#include "debug.h"

void disassembleChunk(Chunk * chunk, const char * name) {
    printf("=== %s ===\n", name);

    for(int offset = 0; offset < chunk->count; offset++) {
        disassembleInstruction(chunk, offset);
    }
}

void disassembleInstruction(Chunk * chunk, int offset) {
    printf("%04d ", offset);

    byte_t instruction = chunk->codes[offset];
    switch(instruction) {
        case OP_RETURN:
            printf("OP_RETURN\n");
            break;
        default:
            printf("Unknown opcode: 0x%02x\n", instruction);
    }
}
