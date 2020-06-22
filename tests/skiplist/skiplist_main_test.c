#include "skiplist.h"

static gint src[] = {5, 1, 16};
static gint src2[] = { 28 };
static gchar *data[] = { "1", "222", "333", "\0" };

static gint comparator(gconstpointer a, gconstpointer b) {
  const gint *aval =  a,
      *bval = b;

  if (*aval > *bval) {
      return 1;
    }
  else if (*aval < *bval) {
      return -1;
    }

  return 0;
}

static void print_list_item(gpointer i, gpointer data)
{
  gint *item = i;
  g_print("%d\n", *item);
}

static void new_list(SList **list, gconstpointer ignored) {
  *list = skiplist_new_full(comparator, NULL);
}

static void free_list(SList **list, gconstpointer ignored) {
  skiplist_destroy(*list);
}

static void create_test(SList **list, gconstpointer data)
{
  // noop
}

static void add_test(SList **list, gconstpointer ignored)
{
  for (gsize i = 0; i < 3; i++) {
    skiplist_add(*list, &src[i]);
  }

  g_print("\n");
  skiplist_foreach(*list, print_list_item, NULL);
}

static void remove_test(SList **list, gconstpointer ignored)
{
  for (gsize i = 0; src[i] != 0; i++) {
    // skiplist_add(*list, &src[i]);
  }
}

int main(int argc, char *argv[])
{
  g_test_init(&argc, &argv, NULL);

  g_test_add("/skiplist/create", SList*, NULL, new_list, create_test, free_list);
  g_test_add("/skiplist/remove", SList*, NULL, new_list, remove_test, free_list);
  g_test_add("/skiplist/add", SList*, NULL, new_list, add_test, free_list);

  return g_test_run();
}
