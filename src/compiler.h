#ifndef KK_COMPILER_H
#define KK_COMPILER_H

#include "scanner.h"
#include "chunk.h"

bool compile(const char * source, Chunk * chunk);

#endif /* KK_COMPILER_H */
