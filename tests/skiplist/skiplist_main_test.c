#include "skiplist.h"
#include <math.h>

static double src[] = {5, 1, 32, 8, 150, 100, 99, NAN};
static double test[] = { 1, 5, 8, 32, 99, 100, 150, NAN };
static double rm_test[] = { 1, 5, 8, 32, 99, 150, NAN };

static gint comparator(gconstpointer a, gconstpointer b) {
  const double *aval =  a,
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
  double *item = i;
  g_print("%f\n", *item);
}

static void match_item(gpointer i, gpointer data)
{
  gsize *j = data;

  g_assert(*((double *) i) == test[*j]);
  (*j)++;
}

static void match_item_remove(gpointer i, gpointer data)
{
  gsize *j = data;

  g_assert(*((double *) i) == rm_test[*j]);
  (*j)++;
}

static void new_list(SList **list, gconstpointer ignored) {
  *list = skiplist_new_full(comparator, NULL);

  for (gsize i = 0; !isnan(src[i]); i++) {
      skiplist_add (*list, &src[i]);
  }
}

static void free_list(SList **list, gconstpointer ignored) {
  skiplist_destroy(*list);
}

static void create_test(SList **list, gconstpointer data)
{
  for (gsize i = 0; !isnan(src[i]); i++) {
      skiplist_add (*list, &src[i]);
  }
}

static void add_test(SList **list, gconstpointer ignored)
{
  gsize counter = 0;
  skiplist_foreach(*list, match_item, &counter);
}

static void found_test(SList **list, gconstpointer ignored)
{
  double target = 100;

  SListItem *val = skiplist_find(*list, &target);

  g_assert(target == *((double *) val->data));
}

static void notfound_test(SList **list, gconstpointer ignored)
{
  double target = 1000;

  SListItem *val = skiplist_find(*list, &target);

  g_assert_null(val);
}

static void remove_test(SList **list, gconstpointer ignored)
{
  double target = 100;

  gshort result = skiplist_remove(*list, &target);

  g_assert(result == 1);

  gsize counter = 0;
  skiplist_foreach(*list, match_item_remove, &counter);
}

int main(int argc, char *argv[])
{
  g_test_init(&argc, &argv, NULL);

  g_test_add("/skiplist/create", SList*, NULL, new_list, create_test, free_list);
  g_test_add("/skiplist/add", SList*, NULL, new_list, add_test, free_list);
  g_test_add("/skiplist/found", SList*, NULL, new_list, found_test, free_list);
  g_test_add("/skiplist/notfound", SList*, NULL, new_list, notfound_test, free_list);
  g_test_add("/skiplist/remove", SList*, NULL, new_list, remove_test, free_list);

  return g_test_run();
}
