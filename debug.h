#ifndef KK_DEBUG_H
#define KK_DEBUG_H

#include "chunk.h"

void disassembleChunk(Chunk * chunk, const char * name);
void disassembleInstruction(Chunk * chunk, int offset);

#endif /* KK_DEBUG_H */
