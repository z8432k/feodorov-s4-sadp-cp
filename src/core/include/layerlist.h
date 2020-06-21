#ifndef _LAYERLIST_H_
#define _LAYERLIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct _LList LList;
typedef struct _LListItem LListItem;

typedef void (*EachCb) (void *data, void *context);

struct _LListItem {
    LListItem *next;
    LList *list;
    void *data;
};

struct _LList {
    LListItem *first;
    EachCb freeCb;
};


inline static void blist_free_item(LListItem *item, void *arg) {
  if (item->list->freeCb) {
      item->list->freeCb(item->data, (void *) NULL);
    }
  printf("\t==> Free memory for list item at %p\n", (void *) item);
  free(item);
}

LList* blist_new(EachCb freeCb);

LListItem* blist_item_new(void *data);

LListItem* blist_add_tail(LList *list, void *data);

LListItem* blist_add_head(LList *list, void *data);

LListItem* blist_add_after(LListItem *item, void *data);

LListItem* blist_remove(LListItem *item);

void blist_each(LList *list, EachCb cb, void *data);

void blist_free(LList *list);

#endif //_LAYERLIST_H_
