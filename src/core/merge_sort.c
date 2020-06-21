#include "merge_sort.h"
#include <memory.h>

#define array_idx_cpy(to, from, to_idx, from_idx, size) \
  (memcpy((char *) to + to_idx * size, (char *) from + from_idx * size, size))

#define array_idx_ptr(arr, idx, size) ((char *) arr + idx * size)

void merge_sort(gpointer values, gpointer buff, gsize size, gsize start, gsize end, merge_sort_comparator comparator)
{
  if (start == end) {
    return;
  }

  gsize mid = (start + end) / 2;

  merge_sort(values, buff, size, start, mid, comparator);
  merge_sort(values, buff, size, mid + 1, end, comparator);

  gsize left_idx = start;
  gsize right_idx = mid + 1;
  gsize buff_idx = left_idx;

  gint compare_result;
  while ((left_idx <= mid) && (right_idx <= end)) {
    compare_result = comparator(
      array_idx_ptr(values, left_idx, size),
      array_idx_ptr(values, right_idx, size)
    );

    if (compare_result <= 0) {
      array_idx_cpy(buff, values, buff_idx++, left_idx++, size);
    }
    else {
      array_idx_cpy(buff, values, buff_idx++, right_idx++, size);
    }
  }

  gsize i;
  for (i = left_idx; i <= mid; i++) {
    array_idx_cpy(buff, values, buff_idx++, i, size);
  }
  for (i = right_idx; i <= end; i++) {
    array_idx_cpy(buff, values, buff_idx++, i, size);
  }
  for (i = start; i <= end; i++) {
    array_idx_cpy(values, buff, i, i, size);
  }
}
