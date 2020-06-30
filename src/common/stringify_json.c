#include "stringify_json.h"

json_t* data_array_to_json_array(GArray *array, jsonFromData builder)
{
  json_t *item;
  json_t *result = json_array();

  for (guint i = 0; i < array->len; i++) {
      void *data = g_array_index(array, void *, i);

      item = builder(data);

      json_array_append_new(result, item);
    }

  return result;
}

json_t* json_build_client(Client_t *data)
{
  json_t *field;
  json_t *obj = json_object();

  field = json_string(data->name->str);
  json_object_set_new(obj, CLIENT_NAME, field);

  field = json_string(data->license->str);
  json_object_set_new(obj, CLIENT_LICENSE, field);

  field = json_string(data->passport->str);
  json_object_set_new(obj, CLIENTS_PASSPORT, field);

  field = json_string(data->address->str);
  json_object_set_new(obj, CLIENTS_ADDRESS, field);

  return obj;
}

json_t* json_build_car(Car_t *data)
{
  json_t *field;
  json_t *obj = json_object();

  field = json_string(data->number->str);
  json_object_set_new(obj, CAR_NUMBER, field);

  field = json_string(data->model->str);
  json_object_set_new(obj, CAR_MODEL, field);

  field = json_string(data->color->str);
  json_object_set_new(obj, CAR_COLOR, field);

  field = json_integer(data->year);
  json_object_set_new(obj, CAR_YEAR, field);

  field = json_boolean(data->exists);
  json_object_set_new(obj, CAR_EXISTS, field);

  return obj;
}

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

void _each_clients(gpointer ptr, gpointer target)
{
  Client_t *client = ptr;
  json_t *arr = target;

  json_array_append(arr, json_build_client(client));
}

gchar* clients_search_stringify_json(GPtrArray *clients)
{
  gchar *str;
  json_t *arr;

  arr = json_array();

  g_ptr_array_foreach(clients, _each_clients, arr);

  str = json_dumps(arr, JSON_INDENT(2));

  json_decref(arr);
  return str;
}
