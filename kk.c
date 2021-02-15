#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"

int main(int argc, const char * argv[]) {
    Chunk chunk;
    initChunk(&chunk);
    writeChunk(&chunk, OP_CONSTANT);
    addConstant(&chunk, 1);
    writeChunk(&chunk, chunk.constants.count - 1); // write index of constants pool
    writeChunk(&chunk, OP_CONSTANT);
    addConstant(&chunk, 100);
    writeChunk(&chunk, chunk.constants.count - 1);
    writeChunk(&chunk, OP_CONSTANT);
    addConstant(&chunk, 1000); // overflow
    writeChunk(&chunk, chunk.constants.count - 1);
    writeChunk(&chunk, OP_RETURN);
    disassembleChunk(&chunk, "test chunk");
    resetChunk(&chunk);
    return 0;
}
