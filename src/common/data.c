#include "data.h"

#include <psql_store.h>

// Raw data
RawData_t* new_data()
{
  RawData_t *data = g_new(RawData_t, 1);

  data->clients = g_array_new(TRUE, TRUE, sizeof(Client_t *));
  g_array_set_clear_func(data->clients, (GDestroyNotify) free_client);

  data->cars = g_array_new(TRUE, TRUE, sizeof(Car_t *));
  g_array_set_clear_func(data->cars, (GDestroyNotify) free_car);

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
