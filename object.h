#ifndef KK_OBJECT_H
#define KK_OBJECT_H

typedef enum {
    OBJ_STRING,
} ObjType;

/* Object and ObjString memory model:
 *
 * | | | | | <- Object
 * ---------
 * | type  |
 * | | | | | | | | | | | | | | | | | <-ObjString
 * ---------------------------------
 * |object |length |    chars[]    |
 *
 * NOTE: we can safely convert ObjString* to Object* and access `type` field,
 * becuase the first byte is aligned.
 *
 * add `next` field:
 * | | | | | | | | | | | | | | | | | <-Object
 * ---------------------------------
 * | type  |padding|     next      |
 */
typedef struct Object Object;
struct Object {
    ObjType  type;
    Object * next;
};

extern Object * objects;

typedef struct {
    Object  object;
    int     length;
    char *  chars;
} ObjString;

ObjString * copyString(const char * chars, int length);
ObjString * createString(char * chars, int length);

void printObject(Object * object);
void freeObjects(Object * objects);

#endif /* KK_OBJECT_H */
