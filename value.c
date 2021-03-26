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
        case VAL_OBJECT:    printObject(AS_OBJECT(value)); break;
    }
}

bool isEqual(KkValue a, KkValue b) {
    if(a.type != b.type) return false;

    switch(a.type) {
        case VAL_BOOL:   return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL:    return true;
        case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_OBJECT: {
            Object * aObject = AS_OBJECT(a);
            Object * bObject = AS_OBJECT(b);
            if(aObject->type != bObject->type) return false;

            switch(aObject->type) {
                case OBJ_STRING: {
                    ObjString * aString = AS_STRING(a);
                    ObjString * bString = AS_STRING(b);
                    return aString->length = bString->length &&
                        memcmp(aString->chars, bString->chars,aString->length) == 0;
                }
            }
        }
        default:
            return false; // unreachable.
    }
}

inline bool isObjType(KkValue value, ObjType type) {
    return IS_OBJECT(value) && AS_OBJECT(value)->type == type;
}
