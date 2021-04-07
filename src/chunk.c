#include "chunk.h"
#include "memory.h"

void initChunk(Chunk * chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->codes = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk * chunk, byte_t byte, int line) {
    if(chunk->capacity < chunk->count + 1) {
        int old = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(old);
        chunk->codes = GROW_ARRAY(byte_t, chunk->codes, old, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, old, chunk->capacity);
    }

    chunk->codes[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

void resetChunk(Chunk * chunk) {
    FREE_ARRAY(byte_t, chunk->codes, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    resetValueArray(&chunk->constants);
    initChunk(chunk);
}

int addConstant(Chunk * chunk, KkValue value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}
