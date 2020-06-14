//#ifndef _SORT_H_
//#define _SORT_H_

//#endif //_SORT_H_

#include <glib.h>

void merge_sort(int values[], int buff[], gsize start, gsize end, int (*comparator)(int *, int *));
