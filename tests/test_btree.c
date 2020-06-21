#include <stdio.h>
#include "btree.h"

static gint src[] = {1, 5, 16, 2, 99, 68, 36, 86, 79, 52, 14, 9, 7, 0};

static gint comparator(gconstpointer a, gconstpointer b, gpointer user_data)
{
  return a - b;
}

static gboolean print_node(gint key, gint value, gpointer user_data)
{
  g_print("KEY: %d, VAL: %d\n", (gint) key, (gint) value);

  return FALSE;
}

int main(void)
{
  BTree *tree = btree_new((GCompareDataFunc) comparator);

  for (gsize i = 0; src[i] != 0; i++) {
    btree_insert(tree, (gpointer) src[i], (gpointer) (src[i] + 100));
  }

  btree_foreach(tree, print_node, NULL);

  gint target = btree_lookup(tree, 2);

  gboolean ok = btree_remove(tree, 86);

  btree_foreach(tree, print_node, NULL);

  ok = btree_remove(tree, 52);

  btree_foreach_pre(tree, print_node, NULL);

  target = btree_lookup(tree, 52);

  btree_insert(tree, 99, 100);

  btree_foreach(tree, print_node, NULL);

  g_print("\n");

  btree_foreach_pre(tree, print_node, NULL);

  btree_destroy(tree);
}

