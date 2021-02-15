#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char * argv[]) {
    Chunk chunk;
    initChunk(&chunk);
    writeChunk(&chunk, OP_RETURN);
    writeChunk(&chunk, 2);
    disassembleChunk(&chunk, "test chunk");
    resetChunk(&chunk);
    return 0;
}
