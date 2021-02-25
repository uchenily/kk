#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"
#include "vm.h"

int main(int argc, const char * argv[]) {
    VM * vm = initVM();
    /* OP_CONSTANT */
    writeChunk(vm->chunk, OP_CONSTANT, 1);
    addConstant(vm->chunk, 1);
    writeChunk(vm->chunk, vm->chunk->constants.count - 1, 1); // write index of constants pool
    writeChunk(vm->chunk, OP_CONSTANT, 1);
    addConstant(vm->chunk, 100);
    writeChunk(vm->chunk, vm->chunk->constants.count - 1, 1);
    writeChunk(vm->chunk, OP_CONSTANT, 3);
    addConstant(vm->chunk, 1000); // overflow
    writeChunk(vm->chunk, vm->chunk->constants.count - 1, 3);
    /* OP_NEGATE */
    writeChunk(vm->chunk, OP_NEGATE, 4);
    /* OP_ADD / OP_SUBSTRACT / OP_MULTIPY / OP_DIVIDE */
    addConstant(vm->chunk, 66);
    writeChunk(vm->chunk, OP_CONSTANT, 5);
    writeChunk(vm->chunk, vm->chunk->constants.count -1, 5);

    addConstant(vm->chunk, 600);
    writeChunk(vm->chunk, OP_CONSTANT, 5);
    writeChunk(vm->chunk, vm->chunk->constants.count -1, 5);

    writeChunk(vm->chunk, OP_ADD, 5);

    addConstant(vm->chunk, 222);
    writeChunk(vm->chunk, OP_CONSTANT, 6);
    writeChunk(vm->chunk, vm->chunk->constants.count -1, 6);

    writeChunk(vm->chunk, OP_DIVIDE, 6);
    /* OP_RETURN */
    writeChunk(vm->chunk, OP_RETURN, 999);

    /* debug */
    disassembleChunk(vm->chunk, "test chunk");

    /* execute */
    interpret(vm, vm->chunk);

    resetChunk(vm->chunk);
    resetVM(vm);
    return 0;
}
