# include "include/merge_sort.h"

void merge_sort(int values[], int buff[], gsize start, gsize end, int (*comparator)(int *, int *))
{
  if (start == end) {
    return;
  }

  gsize mid = (start + end) / 2;

  merge_sort(values, buff, start, mid, comparator);
  merge_sort(values, buff, mid + 1, end, comparator);

  gsize left_idx = start;
  gsize right_idx = mid + 1;
  gsize buff_idx = left_idx;

  while ((left_idx <= mid) && (right_idx <= end)) {
    if (values[left_idx] <= values[right_idx]) {
      buff[buff_idx] = values[left_idx++];
    }
    else {
      buff[buff_idx] = values[right_idx++];
    }
    buff_idx++;
  }

  gsize i;
  for (i = left_idx; i <= mid; i++) {
    buff[buff_idx++] = values[i];
  }
  for (i = right_idx; i <= end; i++) {
    buff[buff_idx++] = values[i];
  }
  for (i = start; i <= end; i++) {
    values[i] = buff[i];
  }
}
