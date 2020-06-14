#include <glib.h>

#include "../include/merge_sort.h"

#define SIZE 17

static gint arr[SIZE] = {5, 14, 6, 7, 98, 63, 11, 0, 2, 3, 2, 56, 8, 7, 115, 300, 1};
static gint buff[SIZE];

int main(void) {

  merge_sort(arr, buff, 0, SIZE - 1, NULL);

  gint prev;
  for (size_t i = 0, prev = arr[i]; i < SIZE; i++) {
    if (i > 0) {
      g_assert(prev <= arr[i]);
      prev = arr[i];
    }
  }

  return 0;
}