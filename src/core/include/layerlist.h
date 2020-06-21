#ifndef _LAYERLIST_H_
#define _LAYERLIST_H_

#include <glib.h>

typedef struct _LList LList;
typedef struct _LListItem LListItem;

typedef void (*EachCb) (gpointer data, gpointer context);

struct _LListItem {
    LListItem *next;
    LList *list;
    gpointer data;
};

struct _LList {
    LListItem *first;
    GDestroyNotify item_destroy_func;
};


inline static void blist_free_item(LListItem *item, gpointer arg) {
  if (item->list->item_destroy_func) {
      item->list->item_destroy_func(item->data);
    }
  g_print("\t==> Free memory for list item at %p\n", (gpointer ) item);
  g_free(item);
}

LList* blist_new(GDestroyNotify item_destroy_func);

LListItem* blist_item_new(gpointer data);

LListItem* blist_add_tail(LList *list, gpointer data);

LListItem* blist_add_head(LList *list, gpointer data);

LListItem* blist_add_after(LListItem *item, gpointer data);

LListItem* blist_remove(LListItem *item);

void blist_each(LList *list, EachCb cb, gpointer data);

void blist_free(LList *list);

#endif //_LAYERLIST_H_
