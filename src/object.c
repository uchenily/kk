#include "common.h"
#include "memory.h"
#include "object.h"
#include "values.h"
#include "table.h"
#include "vm.h"

#define ALLOC_OBJECT(structType, objType) \
    (structType *)allocateObject(sizeof(structType), objType)

static Object * allocateObject(int length, ObjType type) {
    // NOTE: the size of allocated memory not just the size of Object itself,
    // there is room for the extra payload fields need by the specific object
    // type being created.
    Object * object = ALLOC(Object, length);
    object->type = type;

    // add allocated object to object list, all memory created by the
    // allocated object will eventually be freed by traverse this list.
    object->next = vm.objects;
    vm.objects   = object;

    return object;
}

static uint32_t hashString(const char * key, int length) {
    uint32_t hash = 2166136261u;

    for(int i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }

    return hash;
}

static ObjString * allocateString(char * chars, int length, uint32_t hash) {
    ObjString * result = ALLOC_OBJECT(ObjString, OBJ_STRING);
    result->length = length;
    result->chars = chars;
    result->hash = hash;

    tableSet(&vm.strings, result, NIL);

    return result;
}

ObjString * copyString(const char * chars, int length) {
    uint32_t hash = hashString(chars, length);
    ObjString * interned = tableFindString(&vm.strings, chars, length, hash);

    if(interned != NULL) return interned;

    char * heapChars = ALLOC(char, sizeof(char) * (length + 1));
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';

    return allocateString(heapChars, length, hash);
}

/* pointer chars should not point to a constant value because if this value
 * is interned and call free(chars) will raise an error. */
ObjString * createString(char * chars, int length) {
    uint32_t hash = hashString(chars, length);
    ObjString * interned = tableFindString(&vm.strings, chars, length, hash);

    if(interned != NULL) {
        FREE_ARRAY(char, chars, length + 1);
        return interned;
    }

    return allocateString(chars, length, hash);
}

void printObject(Object * object) {
    switch(object->type) {
        case OBJ_STRING:
            printf("\"%s\"", ((ObjString *)object)->chars);
            break;
    }
}

static void freeObject(Object * object) {
    switch(object->type) {
        case OBJ_STRING: {
            ObjString * objString = (ObjString *)object;
            FREE_ARRAY(char, objString->chars, objString->length);
            FREE(ObjString, object);
            break;
         }
    }
}

void freeObjects(Object * objects) {
    Object * current = objects;
    while(current != NULL) {
        Object * prev = current;
        current = current->next; // assign before calling free()
        freeObject(prev);
    }
}
