#include "common.h"
#include "memory.h"
#include "object.h"
#include "values.h"
#include "vm.h"

// global object list
Object * objects;

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
    object->next  = objects;
    objects       = object;

    return object;
}

static ObjString * allocateString(char * chars, int length) {
    ObjString * result = ALLOC_OBJECT(ObjString, OBJ_STRING);
    result->length = length;
    result->chars = chars;

    return result;
}

ObjString * copyString(const char * chars, int length) {
    char * heapChars = ALLOC(char, sizeof(char) * (length + 1));
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';

    return allocateString(heapChars, length);
}

ObjString * createString(char * chars, int length) {
    return allocateString(chars, length);
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
            // the pointer type in should be Object* or ObjString* ?
            // are they equivalent?
            // or free() is independent of the pointer type?
            // where is the block size recorded for allocated memory?
            // what's the meaning of "the space pointed to by ptr"?
            FREE(ObjString, object);
            break;
         }
    }
}

void freeObjects(Object * objects) {
    for(Object * object = objects; object != NULL; object = object->next) {
        freeObject(object);
    }
}
