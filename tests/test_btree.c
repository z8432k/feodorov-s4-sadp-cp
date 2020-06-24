#include <stdlib.h>
#include <stdio.h>
#include "avltree.h"

static gint src[] = {1, 5, 16, 2, 99, 68, 36, 86, 79, 52, 14, 9, 7, 0};

static gchar *val = "VAL";

static gint comparator(const gint *a, const gint *b, gpointer user_data)
{
  return *a - *b;
}

static gboolean print_node(gint *key, gchar* value, gpointer user_data)
{
  g_print("KEY: %d, VAL: %s\n", *key, value);

  return FALSE;
}

int main(void)
{
  AVLTree *tree = avltree_new(comparator);

  for (gsize i = 0; src[i] != 0; i++) {
    avltree_add(tree, &src[i], val);
  }

  avltree_foreach_pre(tree, print_node, NULL);

  gchar *val = avltree_lookup(tree, &src[1]);
}

