//#ifndef _SORT_H_
//#define _SORT_H_

//#endif //_SORT_H_

#include <glib.h>

typedef gint (* merge_sort_comparator)(gconstpointer, gconstpointer);

void merge_sort(gpointer values, gpointer buff, gsize size, gsize start, gsize end, merge_sort_comparator comparator);