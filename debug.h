#ifndef KK_DEBUG_H
#define KK_DEBUG_H

#include "chunk.h"
#include "vm.h"

void disassembleChunk(Chunk * chunk, const char * name);
int disassembleInstruction(Chunk * chunk, int offset);

void printStack(VM * vm);

#endif /* KK_DEBUG_H */
