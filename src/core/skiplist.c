#include <stdlib.h>
#include "skiplist.h"

static inline gsize skiplist_new_level()
{
  gsize level;

  for (level = 0; rand() < RAND_MAX/2 && level < SLIST_LEVELS - 1; level++);

  return level;
}

SListItem* skiplist_new_item(SList *list, gpointer data, gsize level)
{
  SListItem *item = g_new0(SListItem, 1);

  item->forward = g_new(SListItem*, level + 1);
  item->data = data;
  item->list = list;

  return item;
}

void skiplis_free_item(SListItem *item)
{
  if (item->list->item_destroy_func) {
    item->list->item_destroy_func (item->data);
  }
  g_free(item->forward);
  g_free(item);
}

SList* skiplist_new_full(GCompareFunc comparator, GDestroyNotify item_destroy_func)
{
  SList *list = g_new0(SList, 1);

  list->comparator = comparator;
  list->item_destroy_func = item_destroy_func;
  list->level = 0;
  list->head = skiplist_new_item(list,NULL, SLIST_LEVELS - 1);

  for (gsize i = 0; i < SLIST_LEVELS; i++) {
    list->head->forward[i] = NIL;
  }

  return list;
}

void skiplist_destroy(SList *list)
{
  skiplis_free_item(list->head);
  g_free(list);
}

SListItem* skiplist_add(SList *list, double *data)
{
  SListItem *update[SLIST_LEVELS];
  SListItem *current = list->head;

  for (gssize cur_lvl = list->level; cur_lvl >= 0; cur_lvl--) {

    SListItem *next = current->forward[cur_lvl];

    while (next != NIL && list->comparator(next->data, data) < 0) {
      current = next;
      next = current->forward[cur_lvl];
    }

    update[cur_lvl] = current;
  }

  SListItem *bottom_next = current->forward[0];

  // Next item is target key
  if (bottom_next != NIL && !list->comparator(bottom_next->data, data)) {
      bottom_next->data = data;

    return bottom_next;
  }

  gsize new_level = skiplist_new_level();
  SListItem *new_item = skiplist_new_item(list, data, new_level);

  if (new_level > list->level) {
    for (gushort i = list->level + 1; i <= new_level; i++) {
      update[i] = NIL;
    }

    list->level = new_level;
  }

  for (gsize i = 0; i <= new_level; i++) {
    new_item->forward[i] = update[i]->forward[i];
    update[i]->forward[i] = new_item;
  }

  return new_item;
}

void skiplist_foreach(SList *list, GFunc cb, gpointer data) {
  SListItem *current = list->head;

  while (current->forward[0] != NIL) {
    cb(current->forward[0]->data, data);
    current = current->forward[0];
  }
}
