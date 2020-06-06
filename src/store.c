#include <stdio.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <jansson.h>

#include "include/client.h"
#include "include/store.h"

#define DATA_FILENAME "car_rent_data.json"

#define CLIENTS_FIELD "clients"
#define CLIENT_NAME "name"
#define CLIENT_LICENSE "license"
#define CLIENTS_PASSPORT "passport"
#define CLIENTS_ADDRESS "address"

static inline json_t* json_build_client(Client_t *data)
{
  json_t *field;
  json_t *client = json_object();

  field = json_string(data->name->str);
  json_object_set_new(client, CLIENT_NAME, field);

  field = json_string(data->license->str);
  json_object_set_new(client, CLIENT_LICENSE, field);

  field = json_string(data->passport->str);
  json_object_set_new(client, CLIENTS_PASSPORT, field);

  field = json_string(data->address->str);
  json_object_set_new(client, CLIENTS_ADDRESS, field);

  return client;
}

static inline void json_load_data(RawData_t *data)
{
  json_t *root, *clients;
  json_error_t error;

  root = json_load_file(DATA_FILENAME, 0, &error);
  clients = json_object_get(root, CLIENTS_FIELD);

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
}

void save_data(RawData_t *data)
{
  FILE *file = g_fopen(DATA_FILENAME, "w+");

  json_t *root, *clients, *client;

  root = json_object();
  clients = json_array();

  for (guint i = 0; i < data->clients->len; i++) {
    Client_t *clientData = g_array_index(data->clients, Client_t *, i);
    client = json_build_client(clientData);
    json_array_append_new(clients, client);
  }

  json_object_set_new(root, CLIENTS_FIELD, clients);

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
