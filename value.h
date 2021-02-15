#ifndef KK_VALUE_H
#define KK_VALUE_H

#include "common.h"

// temporary definition
typedef int KkValue;

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
