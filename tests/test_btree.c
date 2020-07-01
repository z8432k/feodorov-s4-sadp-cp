#include <stdlib.h>
#include <stdio.h>
#include "avltree.h"

static gint src[] = {1, 5, 16, 2, 99, 68, 36, 86, 79, 52, 14, 9, 7, 0};

static gchar *val = "VAL";

static gint comparator(gconstpointer a, gconstpointer b, gpointer user_data)
{
  return *(gint *) a - *(gint *) b;
}

static gboolean print_node(gpointer key, gpointer value, gpointer user_data)
{
  g_print("KEY: %d, VAL: %s\n", * (gint *) key, (gchar *) value);

  return FALSE;
}

int main(void)
{
  AVLTree *tree = avltree_new(comparator);

  for (gsize i = 0; src[i] != 0; i++) {
    avltree_add(tree, &src[i], val);
  }

  avltree_foreach_pre(tree, print_node, NULL);
}

