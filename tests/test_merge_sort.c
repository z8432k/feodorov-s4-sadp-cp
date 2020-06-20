#include <glib.h>

#include "../src/include/merge_sort.h"

#define SIZE 17

static gint arr[SIZE] = {5, 14, 6, 7, 98, 63, 11, 0, 2, 3, 2, 56, 8, 7, 115, 300, 1};
static gint buff[SIZE];

static gint comparator(gconstpointer  a, gconstpointer b) {
  gint *aptr = (gint *) a, *bptr = (gint *) b;

  if (*aptr > *bptr) {
    return 1;
  }
  else if (*aptr < *bptr) {
    return -1;
  }

  return 0;
}

int main(void) {

  merge_sort(arr, buff, sizeof(gint), 0, SIZE - 1, comparator);

  gint *a = (gint *) arr;

  gint prev;
  for (size_t i = 0; i < SIZE; i++) {
    if (i > 0) {
      g_assert(prev <= a[i]);
      prev = arr[i];
    }
    else {
      prev = a[1];
    }
    g_print("%d\n", a[i]);
  }

  return 0;
}