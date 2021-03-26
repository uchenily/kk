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
    switch(value.type) {
        case VAL_BOOL:      printf(AS_BOOL(value) ? "true" : "false"); break;
        case VAL_NIL:       printf("nil"); break;
        case VAL_NUMBER:    printf("<num%04x>", (u_int16_t)AS_NUMBER(value)); break;
    }
}

bool isEqual(KkValue a, KkValue b) {
    if(a.type != b.type) return false;

    switch(a.type) {
        case VAL_BOOL:   return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL:    return true;
        case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
        default:
            return false; // unreachable.
    }
}

