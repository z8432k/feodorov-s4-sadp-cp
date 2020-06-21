#include "rentrow.h"

RentRow_t* new_rent_row()
{
  RentRow_t *row = g_new(RentRow_t, 1);

  row->number = g_string_new(NULL);
  row->license = g_string_new(NULL);
  row->rentdate = g_string_new(NULL);
  row->returndate = g_string_new(NULL);

  return row;
}

void free_rent_row(RentRow_t *row)
{
  g_string_free(row->number, TRUE);
  g_string_free(row->license, TRUE);
  g_string_free(row->rentdate, TRUE);
  g_string_free(row->returndate, TRUE);

  g_free(row);
}

void fill_rent_row(RentRow_t *row, const gchar *license, const gchar *number, const gchar *rentdate, const gchar *returndate) {
  g_string_assign(row->number, number);
  g_string_assign(row->license, license);
  g_string_assign(row->rentdate, rentdate);
  g_string_assign(row->returndate, returndate);
}