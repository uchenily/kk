#ifndef KK_MOMORY_H
#define KK_MOMORY_H

#include "common.h"

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type *)reallocate(pointer, sizeof(type) * (oldCount), \
            sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

// TODO: remove oldSize
void * reallocate(void * pointer, size_t oldSize, size_t newSize);

#define ALLOC(type, count) \
    (type *)reallocate(NULL, 0, count)

#define FREE(type, pointer) reallocate(pointer, sizeof(type), 0)

#endif /* KK_MOMORY_H */
