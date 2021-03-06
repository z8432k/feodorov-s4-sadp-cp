#ifndef _LAYERLIST_H_
#define _LAYERLIST_H_

#include <glib.h>

typedef struct _LList LList;
typedef struct _LListItem LListItem;

struct _LListItem {
  LListItem *next_l2;
  LListItem *next_l1;
  LListItem *next;
  LList *list;
  gpointer data;
};

struct _LList {
  LListItem *first;
  LListItem *last;
  GDestroyNotify item_destroy_func;
};

LList* llist_new(GDestroyNotify item_destroy_func);

LListItem* llist_item_new(gpointer data);

LListItem* llist_add_tail(LList *list, gpointer data);

LListItem* llist_add_head(LList *list, gpointer data);

LListItem* llist_add_after(LListItem *item, gpointer data);

LListItem* llist_remove(LListItem *item);

void llist_foreach(LList *list, GFunc cb, gpointer data);

void llist_destroy(LList *list);

LList* llist_sort(LList *list, GCompareFunc comparator);

#endif //_LAYERLIST_H_
