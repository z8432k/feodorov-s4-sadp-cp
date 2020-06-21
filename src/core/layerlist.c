#include <glib.h>
#include "layerlist.h"

LList* blist_new(EachCb freeCb) {
  LList *list = malloc(sizeof(LList));

  list->first = NULL;
  list->freeCb = freeCb;

  return list;
}

LListItem* blist_item_new(void *data) {
  LListItem *item = malloc(sizeof(LListItem));

  item->next = (void *) NULL;
  item->data = data;

  return item;
}

LListItem* blist_add_tail(LList *list, void *data) {
  LListItem *item = blist_item_new(data);

  item->list = list;

  if (!list->first) {
      list->first = item;
      list->first->next = item;
    }

  return item;
}

LListItem* blist_add_head(LList *list, void *data) {
  LListItem *item = blist_item_new(data);

  item->list = list;

  if (list->first) {
      item->next = list->first;
    }

  list->first = item;

  return item;
}

LListItem* blist_add_after(LListItem *item, void *data) {
  LListItem *newItem = blist_item_new(data);

  newItem->list = item->list;

  if (item->next) {
    newItem->next = item->next;
  }


  item->next = newItem;

  return newItem;
}

LListItem* blist_remove(LListItem *item) {
  if (item->list->first == item) {
      if (item->next) {
          item->list->first = item->next;
        }
      else {
          item->list->first = NULL;
        }
    }

  return item;
}

void blist_each(LList *list, EachCb cb, void *data) {
  LListItem *current = list->first;

  while (current) {
      LListItem *target = current;

      current = current->next;

      cb(target, data);
    }
}

void blist_free(LList *list) {
  blist_each(list, (EachCb) blist_free_item, NULL);
  printf("\t==> Free memory for list at %p\n", (void *) list);
  free(list);
}
