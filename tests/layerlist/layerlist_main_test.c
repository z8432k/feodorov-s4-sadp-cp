#include "layerlist.h"

static gint src[] = {5, 1, 16, 2, 99, 68, 36, 86, 79, 52, 14, 9, 7, 0};

static gint src2[] = { 28 };

static void print_list_item(gpointer i, gpointer data)
{
  LListItem *item = i;
  //g_print("%d\n", *((gint *) item->data));
}

static void new_list(LList **list, gconstpointer ignored) {
  *list = llist_new(NULL);
}

static void free_list(LList **list, gconstpointer ignored) {
  llist_destroy(*list);
}

static void create_test(LList **list, gconstpointer data)
{
  // noop
}

static void insert_head_test(LList **list, gconstpointer ignored)
{
  for (gsize i = 0; src[i] != 0; i++) {
    llist_add_head(*list, &src[i]);
  }

  //g_print("\n");
  llist_foreach(*list, print_list_item, NULL);
}

static void insert_tail_test(LList **list, gconstpointer ignored)
{
  for (gsize i = 0; src[i] != 0; i++) {
    llist_add_tail(*list, &src[i]);
  }

  // g_print("\n");
  llist_foreach(*list, print_list_item, NULL);
}

static void remove_test(LList **list, gconstpointer ignored)
{
  for (gsize i = 0; src[i] != 0; i++) {
    llist_add_tail(*list, &src[i]);
  }

  // g_print("\n");
  // llist_foreach(*list, print_list_item, NULL);
}

static gint comparator(gconstpointer a, gconstpointer b) {
  const LListItem **aptr = (const LListItem **) a,
    **bptr = (const LListItem **) b;
  const gint *aval = (*aptr)->data,
    *bval = (*bptr)->data;

  if (*aval > *bval) {
    return 1;
  }
  else if (*aval < *bval) {
    return -1;
  }

  return 0;
}

static void sort_test(LList **list, gconstpointer ignored)
{
  for (gsize i = 0; src[i] != 0; i++) {
    llist_add_tail(*list, &src[i]);
  }

  llist_sort(*list, comparator);

  //g_print("\n");
  //llist_foreach(*list, print_list_item, NULL);
}

int main(int argc, char *argv[])
{
  g_test_init(&argc, &argv, NULL);

  g_test_add("/layerlist/create", LList *, NULL, new_list, create_test, free_list);
  g_test_add("/layerlist/insert_head", LList *, NULL, new_list, insert_head_test, free_list);
  g_test_add("/layerlist/insert_tail", LList *, NULL, new_list, insert_tail_test, free_list);
  g_test_add("/layerlist/remove", LList *, NULL, new_list, remove_test, free_list);
  g_test_add("/layerlist/sort", LList *, NULL, new_list, sort_test, free_list);

  return g_test_run();
}
