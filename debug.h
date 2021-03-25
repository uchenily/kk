#ifndef KK_DEBUG_H
#define KK_DEBUG_H

#include "chunk.h"
#include "vm.h"
#include "scanner.h"

void disassembleChunk(Chunk * chunk, const char * name);
int disassembleInstruction(Chunk * chunk, int offset);

void printStack(VM * vm);

const char * tokenType(TokenType type);

void printTokens(const char * source);

#endif /* KK_DEBUG_H */
