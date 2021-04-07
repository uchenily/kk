#ifndef KK_VM_H
#define KK_VM_H

#include "chunk.h"
#include "table.h"

#define STACK_MAX 1024

typedef struct {
    Chunk *   chunk;
    byte_t *  pc; /* program counter */
    Object *  objects;
    Table     strings;
    KkValue * stackTop;
    KkValue   stack[STACK_MAX]; /* use a fixed size stack, to keep it sample  */
} VM;

extern VM vm;

void initVM();
void resetVM();

InterpretResult interpret(const char * source);

void push(KkValue value);
KkValue pop();

#endif /* KK_VM_H */
