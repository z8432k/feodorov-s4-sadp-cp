#include "data.h"
#include <wchar.h>
#include <string.h>

#include <psql_store.h>

// Raw data
RawData_t* new_data()
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

void free_data(RawData_t *data)
{
  g_array_free(data->clients, TRUE);
  g_array_free(data->cars, TRUE);

  g_free(data);
}

RawData_t* data_load() {
  return load_data_impl();
}

gssize data_add_client(const Client_t *client)
{
  return add_client_impl(client);
}

gssize data_add_car(const Car_t *car)
{
  return add_car_impl(car);
}

void data_truncate_clients(RawData_t *data)
{
  GArray *empty = g_array_new(TRUE, TRUE, sizeof(Client_t *));

  g_array_free(data->clients, TRUE);

  data->clients = empty;
}


gssize data_drop_cars()
{
  return drop_cars_impl();
}

gssize data_drop_car(const gchar *number)
{
  return drop_car_impl(number);
}

gssize data_service_car(const gchar *number, const gboolean flag)
{
  return service_car_impl(number, flag);
}

gssize data_drop_client(const gchar *license)
{
  return drop_client_impl(license);
}

gssize data_drop_clients()
{
  return drop_clients_impl();
}

gssize data_rent_car(const gchar *license, const gchar *number)
{
  return rent_car_impl(license, number);
}

gssize data_return_car(const gchar *license, const gchar *number)
{
  return return_car_impl(license, number);
}

static gint btree_comparator(gconstpointer a, gconstpointer b, gpointer  data) {
  const gchar *one = a, *two = b;

  return strcmp(one, two);
}

static gint skiplist_comparator(gconstpointer a, gconstpointer b)
{
  const RentRow_t *one = a, *two = b;

  return strcmp(one->number->str, two->number->str);
}

Data_t* structured_data()
{
  Data_t *data = g_new0(Data_t, 1);
  RawData_t *raw = data_load();

  BTree *tree = btree_new(btree_comparator);

  for (gsize i = 0; i < raw->rents->len; i++) {
    Client_t *client = g_array_index(raw->clients, Client_t *, i);
    btree_insert(tree, client->license, client);
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
