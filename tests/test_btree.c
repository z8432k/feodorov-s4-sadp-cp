#include <stdlib.h>
#include <stdio.h>
#include "btree2.h"

static gint src[] = {1, 5, 16, 2, 99, 68, 36, 86, 79, 52, 14, 9, 7, 0};

static gint comparator(gconstpointer a, gconstpointer b, gpointer user_data)
{
  return a - b;
}

static gboolean print_node(gint key, gchar value, gpointer user_data)
{
  g_print("KEY: %d, VAL: %d\n", (gint) key, (gint) value);

  return FALSE;
}

int main(void)
{
  struct avltree *tree = NULL;

  for (gsize i = 0; src[i] != 0; i++) {
    tree = avltree_add(tree, src[i], "aaaa");
  }

  btree2_traverse_in_order(tree, print_node, NULL);
}

