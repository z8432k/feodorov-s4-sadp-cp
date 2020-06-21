#include <glib.h>
#include "layerlist.h"

LList* blist_new(GDestroyNotify item_destroy_func) {
  LList *list = g_new0(LList, 1);

  list->first = NULL;
  list->item_destroy_func = item_destroy_func;

  return list;
}

LListItem* blist_item_new(gpointer data) {
  LListItem *item = g_new0(LListItem, 1);

  item->next = (gpointer ) NULL;
  item->data = data;

  return item;
}

LListItem* blist_add_tail(LList *list, gpointer data) {
  LListItem *item = blist_item_new(data);

  item->list = list;

  if (!list->first) {
      list->first = item;
      list->first->next = item;
    }

  return item;
}

LListItem* blist_add_head(LList *list, gpointer data) {
  LListItem *item = blist_item_new(data);

  item->list = list;

  if (list->first) {
      item->next = list->first;
    }

  list->first = item;

  return item;
}

LListItem* blist_add_after(LListItem *item, gpointer data) {
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

void blist_each(LList *list, EachCb cb, gpointer data) {
  LListItem *current = list->first;

  while (current) {
      LListItem *target = current;

      current = current->next;

      cb(target, data);
    }
}

void blist_free(LList *list) {
  blist_each(list, (EachCb) blist_free_item, NULL);
  g_print("\t==> Free memory for list at %p\n", (gpointer) list);
  g_free(list);
}
