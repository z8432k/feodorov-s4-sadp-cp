#include "layerlist.h"

static gint src[] = {1, 5, 16, 2, 99, 68, 36, 86, 79, 52, 14, 9, 7, 0};

static gint src2[] = { 28 };

static void new_list(LList **list, gconstpointer ignored) {
  *list = llist_new(NULL);

  for (gsize i = 0; src[i] != 0; i++) {
    llist_add_head(*list, &src[i]);
  }
}

static void free_list(LList **list, gconstpointer ignored) {
  llist_destroy(*list);
}

static void create_test(LList **list, gconstpointer data)
{
  // noop
}

static void insert_test(LList **list, gconstpointer ignored)
{
  llist_add_head(*list, &src2[0]);
}

int main(int argc, char *argv[])
{
  g_test_init(&argc, &argv, NULL);

  g_test_add("/layerlist/create", LList *, NULL, new_list, create_test, free_list);
  g_test_add("/layerlist/insert", LList *, NULL, new_list, insert_test, free_list);

  return g_test_run();
}
