#ifndef KK_TABLE_H
#define KK_TABLE_H

#include "common.h"
#include "value.h"

typedef struct {
    ObjString * key;
    KkValue value;
} Entry;

typedef struct {
    int count;
    int capacity;
    Entry * entries;
} Table;

void initTable(Table * table);
void freeTable(Table * table);
bool tableGet(Table * table, ObjString * key, KkValue * value);
bool tableSet(Table * table, ObjString * key, KkValue value);
bool tableDelete(Table * table, ObjString * key);
void tableCopy(Table * from, Table * to);

ObjString * tableFindString(Table * table, const char * chars, int length, uint32_t hash);

#endif /* KK_TABLE_H */
