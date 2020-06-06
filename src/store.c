#include <stdio.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <jansson.h>

#include "include/store.h"

RawData_t *load_data()
{
  RawData_t *data = new_raw_data();

  return data;
}

static inline json_t* json_build_client(Client_t *data)
{
  json_t *field;
  json_t *client = json_object();

  field = json_string(data->name->str);
  json_object_set_new(client, "name", field);

  field = json_string(data->license->str);
  json_object_set_new(client, "license", field);

  field = json_string(data->passport->str);
  json_object_set_new(client, "passport", field);

  field = json_string(data->address->str);
  json_object_set_new(client, "address", field);

  return client;
}

void save_data(RawData_t *data)
{
  FILE *file = g_fopen("car_rent_data.json", "w+");

  json_t *root, *clients, *client;

  root = json_object();
  clients = json_array();

  for (guint i = 0; i < data->clients->len; i++) {
    Client_t *clientData = g_array_index(data->clients, Client_t *, i);
    client = json_build_client(clientData);
    json_array_append_new(clients, client);
  }

  json_object_set_new(root, "clients", clients);

  json_dumpf(root, file, JSON_INDENT(2));

  json_decref(root);
}
