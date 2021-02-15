#ifndef KK_CHUNK_H
#define KK_CHUNK_H

#include "common.h"
#include "value.h"

typedef enum {
    OP_RETURN,
} OpCode;

typedef uint8_t byte_t;

typedef struct {
    byte_t * codes;
    int      count;
    int      capacity;
} Chunk;

void initChunk(Chunk * chunk);
void writeChunk(Chunk * chunk, byte_t byte);
void resetChunk(Chunk * chunk);

#endif /* KK_CHUNK_H */
