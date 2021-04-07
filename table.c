#include "common.h"
#include "memory.h"
#include "object.h"
#include "table.h"

#define TABLE_MAX_LOAD 0.75

void initTable(Table * table) {
    table->count = 0;
    table->capacity = 0;
    table->entries = NULL;
}

void freeTable(Table * table) {
    FREE_ARRAY(Entry, table->entries, table->capacity);
    initTable(table);
}

// different from ch20
static Entry * findEntry(Entry * entries, int capacity, ObjString * key) {
    uint32_t index = key->hash % capacity;
    for(;;) {
        Entry * entry = &entries[index];
        if(entry->key == NULL && IS_NIL(entry->value)) {
            return entry;
        } else if(entry->key == key){
            return entry;
        }

        // tombstone
        // if(entry->key == NULL && IS_BOOL(entry->value)), then go on

        index = (index + 1) % capacity;
    }
}

static void adjustCapacity(Table * table,  int  capacity) {
    Entry * entries = ALLOC(Entry, sizeof(Entry) * capacity);
    for(int i = 0; i < capacity; i++) {
        entries[i].key = NULL;
        entries[i].value = NIL;
    }

    // rebuild hash table from scratch
    table->count = 0;
    for(int i = 0; i < table->capacity; i++) {
        Entry * entry = &table->entries[i];
        if(entry->key == NULL) continue;

        Entry * dest = findEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        table->count++;
    }
    // release old array's memory
    FREE_ARRAY(Entry, table->entries, table->capacity);

    table->entries = entries;
    table->capacity = capacity;
}

bool tableGet(Table * table, ObjString * key, KkValue * value) {
    if(table->count == 0) return false;

    Entry * entry = findEntry(table->entries, table->capacity, key);
    if(entry->key == NULL) return false;

    *value = entry->value;
    return true;
}

bool tableSet(Table * table, ObjString * key, KkValue value) {
    if(table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
        int capacity = GROW_CAPACITY(table->capacity);
        adjustCapacity(table, capacity);
    }

    Entry * entry = findEntry(table->entries, table->capacity, key);

    bool isNewKey = entry->key == NULL;
    if(isNewKey && IS_NIL(entry->value)) table->count++;

    entry->key = key;
    entry->value = value;
    return isNewKey;
}

/* we treat tombstones like full buckets rather than empty buckets becuase not
 * having an autual empty bucket will introduce an infinite loop problem.
 * so, we do not reduce the table count here. */
bool tableDelete(Table * table, ObjString * key) {
    if(table->count == 0) return false;

    Entry * entry = findEntry(table->entries, table->capacity, key);
    if(entry->key == NULL) return false;

    // place a tombstone in the entry
    // {NULL: true}
    entry->key = NULL;
    entry->value = BOOL(true);

    return true;
}

void tableCopy(Table * from, Table * to) {
    for(int i = 0; i< from->capacity; i++) {
        Entry * entry = &from->entries[i];
        if(entry->key != NULL) {
            tableSet(to, entry->key, entry->value);
        }
    }
}

ObjString * tableFindString(Table * table, const char * chars, int length,
        uint32_t hash) {
    if(table->count == 0) return NULL;

    uint32_t index = hash % table->capacity;
    for(;;) {
        Entry * entry = &table->entries[index];
        if(entry->key == NULL && IS_NIL(entry->value)) {
            return NULL;
        } else if(entry->key->length == length &&
                entry->key->hash == hash &&
                memcmp(entry->key->chars, chars, length) == 0) {
            return entry->key;
        }

        index = (index + 1) % table->capacity;
    }
}
