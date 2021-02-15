#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"

int main(int argc, const char * argv[]) {
    Chunk chunk;
    initChunk(&chunk);
    writeChunk(&chunk, OP_CONSTANT, 1);
    addConstant(&chunk, 1);
    writeChunk(&chunk, chunk.constants.count - 1, 1); // write index of constants pool
    writeChunk(&chunk, OP_CONSTANT, 1);
    addConstant(&chunk, 100);
    writeChunk(&chunk, chunk.constants.count - 1, 1);
    writeChunk(&chunk, OP_CONSTANT, 3);
    addConstant(&chunk, 1000); // overflow
    writeChunk(&chunk, chunk.constants.count - 1, 3);
    writeChunk(&chunk, OP_RETURN, 4);
    disassembleChunk(&chunk, "test chunk");
    resetChunk(&chunk);
    return 0;
}
