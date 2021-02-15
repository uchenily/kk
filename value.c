#include "value.h"
#include "memory.h"

void initValueArray(KkValueArray * array) {
    array->count = 0;
    array->capacity = 0;
    array->values = NULL;
}

void writeValueArray(KkValueArray * array, KkValue value) {
    if(array->capacity < array->count + 1) {
        int old = array->capacity;
        array->capacity = GROW_CAPACITY(old);
        array->values = GROW_ARRAY(KkValue, array->values, old, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}

void resetValueArray(KkValueArray * array) {
    FREE_ARRAY(KkValue, array->values, array->capacity);
    initValueArray(array);
}

void printValue(KkValue value) {
    printf("%d", value);
}
