#include "chunk.h"
#include "memory.h"

void initChunk(Chunk * chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->codes = NULL;
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk * chunk, byte_t byte) {
    if(chunk->capacity < chunk->count + 1) {
        int old = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(old);
        chunk->codes = GROW_ARRAY(byte_t, chunk->codes, old, chunk->capacity);
    }

    chunk->codes[chunk->count] = byte;
    chunk->count++;
}

void resetChunk(Chunk * chunk) {
    FREE_ARRAY(byte_t, chunk->codes, chunk->capacity);
    initChunk(chunk);
}

void addConstant(Chunk * chunk, KkValue value) {
    writeValueArray(&chunk->constants, value);
}
