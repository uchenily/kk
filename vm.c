#include "common.h"
#include "vm.h"
#include "memory.h"
#include "value.h"
#include "debug.h"
#include "compiler.h"

// global variable
VM vm;

void resetStack() {
    vm.stackTop = vm.stack;
}

void runtimeError(const char * format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    int instruction = vm.pc - vm.chunk->codes - 1;
    int line = vm.chunk->lines[instruction];
    fprintf(stderr, "[line %d] in script\n", line);

    resetStack();
}

void initVM() {
    vm.chunk = ALLOC(Chunk, sizeof(Chunk));
    initChunk(vm.chunk);
    vm.objects = NULL;
    resetStack();
}

void resetVM() {
    freeObjects(vm.objects);
    FREE(Chunk, vm.chunk);
}

void push(KkValue value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

KkValue pop() {
    vm.stackTop--;
    KkValue value = *vm.stackTop;
    return value;
}

KkValue peek(int distance) {
    return vm.stackTop[-1 - distance];
}

/* NOTE:
 * 1. handle the input exception in the BINARY_OP/BINARY_OP_STRING macro
 * 2. use api such as IS_INTEGER(a) rather than a.integer(internal implementation)
 */
#define MAKE_COMPARATOR(name, operator) \
    static KkValue name(KkValue a, KkValue b) { \
        if(IS_INTEGER(a) && IS_INTEGER(b)) { \
            return BOOL(AS_INTEGER(a) operator AS_INTEGER(b)); \
        } else if(IS_INTEGER(a) && IS_FLOAT(b)) { \
            return BOOL(AS_INTEGER(a) operator AS_FLOAT(b)); \
        } else if(IS_FLOAT(a) && IS_INTEGER(b)){ \
            return BOOL(AS_FLOAT(a) operator AS_INTEGER(b)); \
        } else { \
            return BOOL(AS_FLOAT(a) operator AS_FLOAT(b)); \
        } \
    }

MAKE_COMPARATOR(greater, >)
MAKE_COMPARATOR(less, <)

#define MAKE_BINARY_OPERATER(name, operator) \
    KkValue name(KkValue a, KkValue b) { \
        if(IS_INTEGER(a) && IS_INTEGER(b)) { \
            return INTEGER(AS_INTEGER(a) operator AS_INTEGER(b)); \
        } else if(IS_INTEGER(a) && IS_FLOAT(b)) { \
            return FLOAT((double)AS_INTEGER(a) operator AS_FLOAT(b)); \
        } else if(IS_FLOAT(a) && IS_INTEGER(b)) { \
            return FLOAT(AS_FLOAT(a) operator (double)AS_INTEGER(b)); \
        } else { \
            return FLOAT(AS_FLOAT(a) operator AS_FLOAT(b)); \
        } \
    }

MAKE_BINARY_OPERATER(add, +)
MAKE_BINARY_OPERATER(substract, -)
MAKE_BINARY_OPERATER(multiply, *)
MAKE_BINARY_OPERATER(divide, /)

KkValue str_add(KkValue a, KkValue b) {
    ObjString * aObj = AS_STRING(a);
    ObjString * bObj = AS_STRING(b);
    int length = aObj->length + bObj->length;
    char * chars = ALLOC(char, length + 1);
    memcpy(chars, aObj->chars, aObj->length);
    memcpy(chars + aObj->length, bObj->chars, bObj->length);
    chars[length] = '\0';

    ObjString * result = createString(chars, length);
    return OBJECT(result);
}

InterpretResult run() {
/* those macro definitions are only used inside run(), to make that scoping
 * more explicit */
#define READ_BYTE() (*vm.pc++) /* dereference and then increment */
#define READ_CONSTANT(index) (vm.chunk->constants.values[index])

#define BINARY_OP(op) \
    do { \
        if(!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
            runtimeError("Operands must be two numbers."); \
            return KK_RUNTIME_ERROR; \
        } \
        KkValue b = pop(); \
        KkValue a = pop(); \
        push(op(a, b)); \
    } while(0);

#define BINARY_OP_STRING(op) \
    do { \
        if(!IS_STRING(peek(0)) || !IS_STRING(peek(1))) { \
            runtimeError("Operands must be two strings."); \
            return KK_RUNTIME_ERROR; \
        } \
        KkValue b = pop(); \
        KkValue a = pop(); \
        push(op(a, b)); \
    } while(0);

    for(;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printStack();
        int offset = vm.pc - vm.chunk->codes; // (vm.pc - vm.chunk->codes) / sizeof(byte_t)
        disassembleInstruction(vm.chunk, offset);
#endif /* DEBUG_TRACE_EXECUTION */
        byte_t instruction = READ_BYTE();
        switch(instruction) {
            case OP_CONSTANT: {
                int index = READ_BYTE();
                KkValue constant = READ_CONSTANT(index);
                push(constant);
                break;
            }
            case OP_FALSE: {
                push(BOOL(false));
                break;
            }
            case OP_TRUE: {
                push(BOOL(true));
                break;
            }
            case OP_NIL: {
                push(NIL);
                break;
            }
            case OP_GREATER: {
                BINARY_OP(greater);
                break;
            }
            case OP_LESS: {
                BINARY_OP(less);
                break;
            }
            case OP_NEGATE: {
                KkValue value = peek(0);
                if(!IS_NUMBER(value)) {
                    runtimeError("Operand must be a number.");
                    return KK_RUNTIME_ERROR;
                }
                if(IS_INTEGER(value)) push(INTEGER(-AS_INTEGER(pop())));
                if(IS_FLOAT(value)) push(FLOAT(-AS_FLOAT(pop())));
                break;
            }
            case OP_ADD: {
                // peek(1) maybe better than peek(0) because of reading habits
                if(IS_STRING(peek(1))) {
                    BINARY_OP_STRING(str_add);
                } else if(IS_NUMBER(peek(1))) {
                    BINARY_OP(add);
                }
                break;
            }
            case OP_SUBSTRACT: {
                BINARY_OP(substract);
                break;
            }
            case OP_MULTIPLY: {
                BINARY_OP(multiply);
                break;
            }
            case OP_DIVIDE: {
                BINARY_OP(divide);
                break;
            }
            case OP_NOT: {
                KkValue k_value = pop();
                push(IS_ZERO(k_value) ? BOOL(true) : BOOL(false));
                break;
            }
            case OP_EQUAL: {
                KkValue b = pop();
                KkValue a = pop();
                if(a.type != b.type) {
                    runtimeError("Values of different types should not be compared.");
                    return KK_RUNTIME_ERROR;
                }
                push(isEqual(a, b) ? BOOL(true) : BOOL(false));
                break;
            }
            case OP_RETURN: {
                printf("return ");
                printValue(pop());
                printf("\n");
                return KK_OK;
            }
            default: {
                printf("Unknown opcode: 0x%02x", instruction);
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
#undef BINARY_OP_STRING
}

InterpretResult interpret(const char * source) {
    if(!compile(source, vm.chunk)) {
        resetChunk(vm.chunk);
        return KK_COMPILE_ERROR;
    }

    vm.pc = vm.chunk->codes;

    InterpretResult result = run();
    // reset vm.chunk after interpreter executed.
    resetChunk(vm.chunk);
    return result;
}
