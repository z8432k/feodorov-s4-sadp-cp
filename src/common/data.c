#include <string.h>

#include "data.h"
#include "storage.h"

static gint avltree_comparator(gconstpointer a, gconstpointer b, gpointer  data) {
  const gchar *one = a, *two = b;

  return strcmp(one, two);
}

static gint skiplist_comparator(gconstpointer a, gconstpointer b)
{
  const RentRow_t *one = a, *two = b;

  return strcmp(one->number->str, two->number->str);
}

// Raw data
RawData_t* data_new()
{
  RawData_t *data = g_new(RawData_t, 1);

  data->clients = g_array_new(TRUE, TRUE, sizeof(Client_t *));
  g_array_set_clear_func(data->clients, (GDestroyNotify) free_client);

  data->cars = g_array_new(TRUE, TRUE, sizeof(Car_t *));
  g_array_set_clear_func(data->cars, (GDestroyNotify) free_car);

  data->rents = g_array_new(TRUE, TRUE, sizeof(RentRow_t *));
  g_array_set_clear_func(data->rents, (GDestroyNotify) free_rent_row);

  return data;
}

void data_free(RawData_t *data)
{
  g_array_free(data->clients, TRUE);
  g_array_free(data->cars, TRUE);

  g_free(data);
}

Data_t* structured_data()
{
  Data_t *data = g_new0(Data_t, 1);
  RawData_t *raw = data_load();

  AVLTree *tree = avltree_new(avltree_comparator);

  for (gsize i = 0; i < raw->clients->len; i++) {
    Client_t *client = g_array_index(raw->clients, Client_t *, i);
    avltree_add(tree, client->license->str, client);
  }

  data->clients = tree;

  HashTab_t *tab = hash_tab_new();

  for (gsize i = 0; i < raw->cars->len; i++) {
    Car_t *car = g_array_index(raw->cars, Car_t *, i);
    hash_tab_set(tab, car->number, car);
  }

  data->cars = tab;

  SList *list = skiplist_new_full(skiplist_comparator, NULL);

  for (gsize i = 0; i < raw->rents->len; i++) {
    RentRow_t *row = g_array_index(raw->rents, RentRow_t *, i);

    if (row->returndate->len == 0) {
      skiplist_add(list, row);
    }
  }

  data->rents = list;
  return data;
}
