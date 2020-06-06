#include <stdio.h>
#include <glib.h>

#include "include/store.h"

// Raw data
RawData_t* new_raw_data()
{
  RawData_t *data = g_new(RawData_t, 1);

  data->clients = g_array_new(TRUE, TRUE, sizeof(Client_t *));
  g_array_set_clear_func(data->clients, free_client);

  return data;
}


void free_raw_data(RawData_t *data)
{
  g_array_free(data->clients, TRUE);

  g_free(data);
}

// Client
Client_t* new_client()
{
  Client_t *client = g_new(Client_t, 1);

  client->name = g_string_new(NULL);
  client->license = g_string_new(NULL);
  client->passport = g_string_new(NULL);
  client->address = g_string_new(NULL);

  return client;
}

void free_client(gpointer data)
{
  Client_t **client = (Client_t **) data;

  g_string_free((*client)->name, TRUE);
  g_string_free((*client)->license, TRUE);
  g_string_free((*client)->passport, TRUE);
  g_string_free((*client)->address, TRUE);

  g_free(*client);
}

void data_add_client(RawData_t *data, Client_t *client)
{
  g_array_append_val(data->clients, client);
}

void reg_client(Client_t *client)
{
  RawData_t *data = load_data();

  data_add_client(data, client);

  save_data(data);

  free_raw_data(data);
}
