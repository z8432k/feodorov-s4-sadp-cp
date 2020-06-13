#include <stdio.h>
#include <glib/gstdio.h>
#include <jansson.h>

#include "include/json_store.h"

#define DATA_FILENAME "car_rent_data.json"

#define CLIENTS_FIELD "clients"
#define CARS_FIELD "cars"

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

  field = json_integer(data->exists);
  json_object_set_new(obj, CAR_EXISTS, field);

  return obj;
}

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

static inline void json_load_data(RawData_t *data)
{
  json_t *root, *clients, *cars;
  json_error_t error;

  root = json_load_file(DATA_FILENAME, 0, &error);
  clients = json_object_get(root, CLIENTS_FIELD);
  cars = json_object_get(root, CARS_FIELD);

  size_t index;
  json_t *value;

  Client_t *client;
  json_array_foreach(clients, index, value) {
    client = new_client();

    fill_client(client,
      json_string_value(json_object_get(value, CLIENT_NAME)),
      json_string_value(json_object_get(value, CLIENT_LICENSE)),
      json_string_value(json_object_get(value, CLIENTS_PASSPORT)),
      json_string_value(json_object_get(value, CLIENTS_ADDRESS))
    );

    g_array_append_val(data->clients, client);
  }

  Car_t *car;
  json_array_foreach(cars, index, value) {
    car = new_car();

    fill_car(car,
      json_string_value(json_object_get(value, CAR_NUMBER)),
      json_string_value(json_object_get(value, CAR_MODEL)),
      json_string_value(json_object_get(value, CAR_COLOR)),
      json_integer_value(json_object_get(value, CAR_YEAR)),
      json_integer_value(json_object_get(value, CAR_EXISTS))
    );

    g_array_append_val(data->cars, car);
  }
}

void save_data(RawData_t *data)
{
  FILE *file = g_fopen(DATA_FILENAME, "w+");

  json_t *root, *clients, *cars;

  root = json_object();

  clients = clients_array_to_json_array(data->clients, json_build_client);
  json_object_set_new(root, CLIENTS_FIELD, clients);

  cars = cars_array_to_json_array(data->cars, json_build_car);
  json_object_set_new(root, CARS_FIELD, cars);

  json_dumpf(root, file, JSON_INDENT(2));

  json_decref(root);
}

RawData_t *load_data()
{
  RawData_t *data = new_data();

  if (g_file_test(DATA_FILENAME, G_FILE_TEST_EXISTS)) {
    json_load_data(data);
  }

  return data;
}
