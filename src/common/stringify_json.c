#include "stringify_json.h"
#include "json_store.h"

gchar* clients_stringify_json(RawData_t *data)
{
  gchar *str;
  json_t *clients;

  clients = clients_array_to_json_array(data->clients, json_build_client);
  str = json_dumps(clients, JSON_INDENT(2));

  json_decref(clients);

  return str;
}

gchar* cars_stringify_json(RawData_t *data)
{
  gchar *str;
  json_t *cars;

  cars = cars_array_to_json_array(data->cars, json_build_car);
  str = json_dumps(cars, JSON_INDENT(2));

  json_decref(cars);

  return str;
}