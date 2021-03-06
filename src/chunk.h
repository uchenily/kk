#ifndef KK_CHUNK_H
#define KK_CHUNK_H

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_NEGATE,
    OP_ADD,
    OP_SUBSTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NOT,
    OP_RETURN,
} OpCode;

typedef uint8_t byte_t;

typedef struct {
    byte_t *     codes;
    int *        lines; // store line number information
    int          count;
    int          capacity;
    KkValueArray constants;
} Chunk;

void initChunk(Chunk * chunk);
void writeChunk(Chunk * chunk, byte_t byte, int line);
void resetChunk(Chunk * chunk);

int addConstant(Chunk * chunk, KkValue value);

#endif /* KK_CHUNK_H */
