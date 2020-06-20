#include <glib.h>
#include "btree.h"

static gint src[] = {1, 5, 16, 2, 99, 68, 36, 86, 79, 52, 14, 9, 7, 0};

static gint comparator(gconstpointer a, gconstpointer b, gpointer user_data)
{
  return a - b;
}

static void new_tree(BTree **pTree, gconstpointer pVoid) {
  *pTree = btree_new(comparator);
}

static void free_tree(BTree **pTree, gconstpointer pVoid) {
  btree_destroy(*pTree);
}

static void new_test(BTree **tree, gconstpointer *ignored)
{
  g_assert((*tree)->comparator == comparator);
}

static void append_test()
{

}

int main(int argc, char *argv[])
{
  g_test_init(&argc, &argv, NULL);

  g_test_add("/btree/new_destroy", BTree*, NULL, new_tree, new_test, free_tree);
  g_test_add("/btree/append", BTree*, NULL, new_tree, append_test, free_tree);

  return g_test_run();
}