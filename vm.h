#ifndef KK_VM_H
#define KK_VM_H

#include "chunk.h"

#define STACK_MAX 1024

typedef struct {
    Chunk *   chunk;
    byte_t *  pc; /* program counter */
    KkValue * stackTop;
    // Flexible Array Member(FAM)
    // variable length array must be last member of structure
    KkValue   stack[]; /* use a fixed size stack, to keep it sample  */
} VM;

VM * initVM();
void resetVM(VM * vm);

InterpretResult interpret(VM * vm, const char * source);

void push(VM * vm, KkValue value);
KkValue pop(VM * vm);

#endif /* KK_VM_H */
