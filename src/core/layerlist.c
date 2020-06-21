#include <glib.h>
#include "layerlist.h"
#include "merge_sort.h"

static gint comparator(gconstpointer a, gconstpointer b, gpointer user_data)
{
  const gint *one = a;
  const gint *two = b;

  return *one - *two;
}

LList* llist_new(GDestroyNotify item_destroy_func) {
  LList *list = g_new0(LList, 1);

  list->first = NULL;
  list->last = NULL;
  list->item_destroy_func = item_destroy_func;

  return list;
}

LListItem* llist_item_new(gpointer data) {
  LListItem *item = g_new0(LListItem, 1);

  item->data = data;

  return item;
}

LListItem* llist_add_tail(LList *list, gpointer data) {
  LListItem *item = llist_item_new(data);

  item->list = list;

  if (!(list->first || list->last)) {
      list->first = item;
      list->last = item;
    }
  else {
    list->last->next = item;
    list->last = item;
  }

  return item;
}

LListItem* llist_add_head(LList *list, gpointer data) {
  LListItem *item = llist_item_new(data);

  item->list = list;

  if (!(list->first || list->last)) {
    list->first = item;
    list->last = item;
  }
  else {
    item->next = list->first;
    list->first = item;
  }

  return item;
}

LListItem* llist_add_after(LListItem *item, gpointer data) {
  LListItem *newItem = llist_item_new(data);

  newItem->list = item->list;

  if (item->next) {
    newItem->next = item->next;
  }


  item->next = newItem;

  return newItem;
}

LListItem* llist_remove(LListItem *item) {
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

void llist_foreach(LList *list, GFunc cb, gpointer data) {
  LListItem *current = list->first;

  while (current) {
    LListItem *target = current;

    current = current->next;

    cb(target, data);
  }
}

void llist_destroy(LList *list) {
  llist_foreach(list, llist_free_item, NULL);
  g_print("\t==> Free memory for list at %p\n", (gpointer) list);
  g_free(list);
}

LList* llist_sort(LList *list, GCompareFunc comparator)
{
  GPtrArray *array = g_ptr_array_new();
  GPtrArray *buff = g_ptr_array_new();

  LListItem *next = list->first;
  while (next) {
    g_ptr_array_add(array, next);
    g_ptr_array_add(buff, next);

    next = next->next;
  }

  merge_sort(array->pdata, buff->pdata, sizeof(gpointer *), 0, buff->len - 1, comparator);

  list->first = array->pdata[0];
  LListItem *current = list->first;
  for (gsize i = 1; i < array->len; i++) {
    current->next = array->pdata[i];
    current = current->next;
    list->last = current;
  }

  list->last->next = NULL;

  g_ptr_array_free(array, TRUE);
  g_ptr_array_free(buff, TRUE);

  return list;
}
