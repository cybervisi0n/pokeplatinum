#ifndef POKEPLATINUM_STRING_LIST_H
#define POKEPLATINUM_STRING_LIST_H

#include <nitro/types.h>

#include "message.h"
#include "string_gf.h"

#define STRING_LIST_TERMINATOR ((void *)0xFFFFFFFF)

typedef uPtr list_index_t;

typedef struct StringList {
    void *entry;
    union {
        list_index_t index;
        void *callbackFunction;
    };
} StringList;

StringList *StringList_New(u32 capacity, u32 heapID);
void StringList_Free(StringList *list);
void StringList_AddFromMessageBank(StringList *list, const MessageLoader *loader, u32 bankEntry, list_index_t index);
void StringList_AddFromString(StringList *list, const String *string, list_index_t index);
void StringList_AddFromEntry(StringList *list, const StringList *entry);

#endif // POKEPLATINUM_STRING_LIST_H
