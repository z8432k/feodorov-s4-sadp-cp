#include <stdio.h>
#include "../include/btree.h"

static gint src[] = {1, 5, 16, 2, 99, 68, 36, 86, 79, 52, 14, 68, 9, 1, 7, 0};

static gint comparator(gint a, gint b, gpointer user_data)
{
  return a - b;
}

int main(void)
{
  BTree *tree = btree_new((GCompareDataFunc) comparator);

  for (gsize i = 0; src[i] != 0; i++) {
    btree_insert(tree, (gpointer) src[i], (gpointer) (src[i] + 100));
  }
}

