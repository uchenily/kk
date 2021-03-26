#ifndef KK_VALUE_H
#define KK_VALUE_H

#include "common.h"
#include "object.h"

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
    VAL_OBJECT,
} KkValueType;

/* KkValue struct memory model:
 *
 * | | | | |x|x|x|x| | | | | | | | |  16-byte
 * ---------------------------------
 * | type  |padding|     union     |
 * ---------------------------------
 *                 | | <- boolean
 *                 |               | <- number
 *
 * NOTE: most architectures prefer values be aligned to their size,
 * the union field contains a 8-byte double, so there is a padding.
 */
typedef struct {
    KkValueType type;
    union {
        bool     boolean;
        double   number;
        Object * object;
    };
} KkValue;

// function declarations doesn't include `inline` keyword.
bool isObjType(KkValue value, ObjType type);

#define AS_BOOL(k_value)      ((k_value).boolean)
#define AS_NUMBER(k_value)    ((k_value).number)
#define AS_OBJECT(k_value)    ((k_value).object)
#define AS_STRING(k_value)    ((ObjString *)AS_OBJECT(k_value))

#define BOOL(c_value)         ((KkValue){VAL_BOOL, {.boolean = c_value}})
#define NIL                   ((KkValue){VAL_NIL, {.number = 0}})
#define NUMBER(c_value)       ((KkValue){VAL_NUMBER, {.number = c_value}})
// c_object contains `object` field, such as instance of struct ObjString
// use argument name `object` will intruduce a naming conflict here
#define OBJECT(c_object)      ((KkValue){VAL_OBJECT, {.object = (Object *)c_object}})

#define IS_BOOL(k_value)      ((k_value).type == VAL_BOOL)
#define IS_NIL(k_value)       ((k_value).type == VAL_NIL)
#define IS_NUMBER(k_value)    ((k_value).type == VAL_NUMBER)
#define IS_OBJECT(k_value)    ((k_value).type == VAL_OBJECT)
#define IS_STRING(k_value)    isObjType(k_value, OBJ_STRING)

#define IS_ZERO(k_value)      ((k_value).number == 0)


bool isEqual(KkValue a, KkValue b);

typedef enum {
    KK_COMPILE_ERROR,
    KK_RUNTIME_ERROR,
    KK_OK,
} InterpretResult;

typedef struct {
    int count;
    int capacity;
    KkValue * values;
} KkValueArray;

void initValueArray(KkValueArray * array);
void writeValueArray(KkValueArray * array, KkValue value);
void resetValueArray(KkValueArray * array);

void printValue(KkValue value);

#endif /* KK_VALUE_H */
