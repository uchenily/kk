#include "common.h"
#include "vm.h"
#include "memory.h"
#include "value.h"
#include "debug.h"
#include "compiler.h"

void resetStack(VM * vm) {
    vm->stackTop = vm->stack;
}

VM * initVM() {
    // VM * vm = (VM *)malloc(sizeof(VM) + sizeof(KkValue) * STACK_MAX);
    // vm->chunk = (Chunk *)malloc(sizeof(Chunk));
    VM * vm = ALLOC(VM, sizeof(VM) + sizeof(KkValue) * STACK_MAX);
    vm->chunk = ALLOC(Chunk, sizeof(Chunk));
    initChunk(vm->chunk);
    resetStack(vm);
    return vm;
}

// TODO
void resetVM(VM * vm) {

}

void push(VM * vm, KkValue value) {
    *vm->stackTop = value;
    vm->stackTop++;
}

KkValue pop(VM * vm) {
    vm->stackTop--;
    KkValue value = *vm->stackTop;
    return value;
}

KkValue run(VM * vm) {
/* those macro definitions are only used inside run(), to make that scoping
 * more explicit */
#define READ_BYTE() (*vm->pc++) /* ereference and then increment */
#define READ_CONSTANT() (vm->chunk->constants.values[READ_BYTE()])
#define BINARY_OP(vm, op) \
    do { \
        KkValue b = pop(vm); \
        KkValue a = pop(vm); \
        push(vm, a op b); \
    } while(0);

    for(;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printStack(vm);
        int offset = vm->pc - vm->chunk->codes; // (vm->pc - vm->chunk->codes) / sizeof(byte_t)
        disassembleInstruction(vm->chunk, offset);
#endif /* DEBUG_TRACE_EXECUTION */
        byte_t instruction = READ_BYTE();
        switch(instruction) {
            case OP_CONSTANT: {
                KkValue constant = READ_CONSTANT();
                push(vm, constant);
                break;
            }
            case OP_NEGATE: {
                push(vm, -pop(vm));
                break;
            }
            case OP_ADD: {
                BINARY_OP(vm, +);
                break;
            }
            case OP_SUBSTRACT: {
                BINARY_OP(vm, -);
                break;
            }
            case OP_MULTIPLY: {
                BINARY_OP(vm, *);
                break;
            }
            case OP_DIVIDE: {
                BINARY_OP(vm, /);
                break;
            }
            case OP_RETURN: {
                printf("return ");
                printValue(pop(vm));
                printf("\n");
                return 0;
            }
            default: {
                printf("Unknown opcode: 0x%02x", instruction);
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

KkValue interpret(VM * vm, const char * source) {
    compile(source);
    return KK_OK;
}


