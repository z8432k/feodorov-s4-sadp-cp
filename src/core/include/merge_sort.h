//#ifndef _SORT_H_
//#define _SORT_H_

//#endif //_SORT_H_

#include <glib.h>

void merge_sort(gpointer values, gpointer buff, gsize size, gsize start, gsize end, GCompareFunc comparator);