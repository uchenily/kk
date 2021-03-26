#include "common.h"
#include "vm.h"
#include "memory.h"
#include "value.h"
#include "debug.h"
#include "compiler.h"

void resetStack(VM * vm) {
    vm->stackTop = vm->stack;
}

void runtimeError(VM * vm, const char * format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    int instruction = vm->pc - vm->chunk->codes - 1;
    int line = vm->chunk->lines[instruction];
    fprintf(stderr, "[line %d] in script\n", line);

    resetStack(vm);
}

VM * initVM() {
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

KkValue peek(VM * vm, int distance) {
    return vm->stackTop[-1 - distance];
}

KkValue greater(KkValue a, KkValue b)   {return BOOL(a.number > b.number);}
KkValue less(KkValue a, KkValue b)      {return BOOL(a.number < b.number);}
KkValue add(KkValue a, KkValue b)       {return NUMBER(a.number + b.number);}
KkValue substract(KkValue a, KkValue b) {return NUMBER(a.number - b.number);}
KkValue multiply(KkValue a, KkValue b)  {return NUMBER(a.number * b.number);}
KkValue divide(KkValue a, KkValue b)    {return NUMBER(a.number / b.number);}

InterpretResult run(VM * vm) {
/* those macro definitions are only used inside run(), to make that scoping
 * more explicit */
#define READ_BYTE() (*vm->pc++) /* dereference and then increment */
#define READ_CONSTANT() (vm->chunk->constants.values[READ_BYTE()])

#define BINARY_OP(vm, op) \
    do { \
        if(!IS_NUMBER(peek(vm, 0)) || !IS_NUMBER(peek(vm, 1))) { \
            runtimeError(vm, "Operands must be numbers."); \
            return KK_RUNTIME_ERROR; \
        } \
        KkValue b = pop(vm); \
        KkValue a = pop(vm); \
        push(vm, op(a, b)); \
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
            case OP_FALSE: {
                push(vm, BOOL(false));
                break;
            }
            case OP_TRUE: {
                push(vm, BOOL(true));
                break;
            }
            case OP_NIL: {
                push(vm, NIL);
                break;
            }
            case OP_GREATER: {
                BINARY_OP(vm, greater);
                break;
            }
            case OP_LESS: {
                BINARY_OP(vm, less);
                break;
            }
            case OP_NEGATE: {
                if(!IS_NUMBER(peek(vm, 0))) {
                    runtimeError(vm, "Operand must be a number.");
                    return KK_RUNTIME_ERROR;
                }
                push(vm, NUMBER(-AS_NUMBER(pop(vm))));
                break;
            }
            case OP_ADD: {
                BINARY_OP(vm, add);
                break;
            }
            case OP_SUBSTRACT: {
                BINARY_OP(vm, substract);
                break;
            }
            case OP_MULTIPLY: {
                BINARY_OP(vm, multiply);
                break;
            }
            case OP_DIVIDE: {
                BINARY_OP(vm, divide);
                break;
            }
            case OP_NOT: {
                KkValue k_value = pop(vm);
                push(vm, IS_ZERO(k_value) ? BOOL(true) : BOOL(false));
                break;
            }
            case OP_EQUAL: {
                KkValue b = pop(vm);
                KkValue a = pop(vm);
                push(vm, isEqual(a, b) ? BOOL(true) : BOOL(false));
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

InterpretResult interpret(VM * vm, const char * source) {
    // initialize vm->chunk each time we run the interpretor.
    initChunk(vm->chunk);

    if(!compile(source, vm->chunk)) {
        return KK_COMPILE_ERROR;
    }

    vm->pc = vm->chunk->codes;

    InterpretResult result = run(vm);
    return result;
}
