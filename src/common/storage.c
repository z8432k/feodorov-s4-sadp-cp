#include <dlfcn.h>
#include <glib.h>
#include "data.h"
#include "storage.h"

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


RawData_t* data_search_car_fragment(const gchar *request)
{
  return search_car_fragment_impl(request);
}
