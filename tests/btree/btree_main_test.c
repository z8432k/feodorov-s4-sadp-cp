#include <glib.h>
#include "btree.h"

static gint src[] = {1, 5, 16, 2, 99, 68, 36, 86, 79, 52, 14, 9, 7, 0};

static gint comparator(gconstpointer a, gconstpointer b, gpointer user_data)
{
  return a - b;
}

static void new_tree(BTree **tree, gconstpointer pVoid) {
  *tree = btree_new(comparator);

  for (gsize i = 0; src[i] != 0; i++) {
    btree_insert(*tree, (gpointer) src[i], (gpointer) (src[i] + 100));
  }
}

static void free_tree(BTree **pTree, gconstpointer ignored) {
  btree_destroy(*pTree);
}

static void new_test(BTree **tree, gconstpointer ignored)
{
  g_assert((*tree)->comparator == comparator);
}

static void insert_test(BTree **tree, gconstpointer ignored)
{
  btree_insert(*tree, 15, 2);
}

static void replace_test(BTree **tree, gconstpointer ignored)
{
  // btree_insert(*tree, 17, 2);
}

int main(int argc, char *argv[])
{
  g_test_init(&argc, &argv, NULL);

  g_test_add("/btree/new_destroy", BTree*, NULL, new_tree, new_test, free_tree);
  g_test_add("/btree/insert", BTree*, NULL, new_tree, insert_test, free_tree);
  // g_test_add("/btree/replace", BTree*, NULL, new_tree, replace_test, free_tree);

  return g_test_run();
}
