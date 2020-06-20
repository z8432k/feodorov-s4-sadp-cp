#include <stdio.h>
#include "../src/include/btree.h"




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

  btree_foreach(tree, print_node, NULL);

  target = btree_lookup(tree, 52);

  btree_destroy(tree);
}

