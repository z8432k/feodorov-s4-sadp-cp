#include "skiplist.h"
#include <math.h>

static double src[] = {5, 1, 32, 8, 150, 100, 99, NAN};
static double test[] = { 1, 5, 8, 32, 99, 100, 150, NAN };

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

static void match_item(double *i, gpointer data)
{
  gsize *j = data;

  g_assert(*i == test[*j]);
  (*j)++;
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
  for (gsize i = 0; !isnan(src[i]); i++) {
    skiplist_add(*list, &src[i]);
  }

  gsize counter = 0;
  skiplist_foreach(*list, match_item, &counter);
}

static void remove_test(SList **list, gconstpointer ignored)
{
  for (gsize i = 0; !isnan((double) src[i]); i++) {
    // skiplist_add(*list, &src[i]);
  }
}

int main(int argc, char *argv[])
{
  g_test_init(&argc, &argv, NULL);

  g_test_add("/skiplist/create", SList*, NULL, new_list, create_test, free_list);
  g_test_add("/skiplist/add", SList*, NULL, new_list, add_test, free_list);
  // g_test_add("/skiplist/remove", SList*, NULL, new_list, remove_test, free_list);

  return g_test_run();
}
