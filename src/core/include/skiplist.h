#ifndef _SKIPLIST_H_
#define _SKIPLIST_H_

#include <glib.h>

#define SLIST_LEVELS 5
#define NIL list->head

typedef struct __SListItem SListItem;

typedef struct __SList {
    SListItem *head;
    gsize level;
    GDestroyNotify item_destroy_func;
    GCompareFunc comparator;
} SList;

typedef struct __SListItem {
    SList *list;
    gpointer *data;
    SListItem **forward;
} SListItem;

SList* skiplist_new_full(GCompareFunc comparator, GDestroyNotify item_destroy_func);

SListItem* skiplist_new_item(SList *list, gpointer data, gsize level);

void skiplis_free_item(SListItem *item);

void skiplist_destroy(SList *list);

SListItem* skiplist_add(SList *list, gpointer data);

SListItem* skiplist_find(SList *list, gconstpointer data);

gshort skiplist_remove(SList *list, gconstpointer data);

void skiplist_foreach(SList *list, GFunc func, gpointer user_data);

#endif //_SKIPLIST_H_
