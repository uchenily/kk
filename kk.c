#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"
#include "vm.h"
#include "memory.h"
#include "string.h"

void repl(VM * vm) {
    char line[1024];
    for(;;) {
        printf("> ");

        if(!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(vm, line);
    }
}

char * readFile(const char * path) {
    FILE * file = fopen(path, "rb");
    if(file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char * buffer = ALLOC(char, fileSize + 1);
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if(bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}

void runFile(VM * vm, const char * path) {
    char * source = readFile(path);
    KkValue result = interpret(vm, source);
    free(source);

    if(result == KK_COMPILE_ERROR) exit(65);
    if(result == KK_RUNTIME_ERROR) exit(70);
}

int main(int argc, const char * argv[]) {
    VM * vm = initVM();
    if(argc == 1) {
        repl(vm);
    } else if(argc == 2) {
        runFile(vm, argv[1]);
    } else {
        fprintf(stderr, "Usage: kk [path]\n");
        exit(64);
    }

    resetVM(vm);
    return 0;
}
