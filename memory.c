#include "memory.h"

void * reallocate(void * pointer, size_t oldSize, size_t newSize) {
    if(newSize == 0) {
        free(pointer);
        return NULL;
    }

    void * result = realloc(pointer, newSize);
    // result could be NULL, but I don't care about that case...
    return result;
}
