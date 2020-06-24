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

gchar* cars_search_stringify_json(Car_t *car, Client_t *client)
{
  gchar *str;
  json_t *obj, *field;

  obj = json_object();

  if (car) {
    field = json_build_car(car);
    json_object_set_new(obj, "car", field);
  }

  if (client) {
    field = json_build_client(client);
    json_object_set_new(obj, "client", field);
  }

  str = json_dumps(obj, JSON_INDENT(2));

  json_decref(obj);
  return str;
}
