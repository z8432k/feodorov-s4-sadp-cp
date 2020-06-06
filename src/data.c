#include <stdio.h>
#include <glib.h>

#include "include/client.h"
#include "include/store.h"

// Raw data
RawData_t* new_data()
{
  RawData_t *data = g_new(RawData_t, 1);

  data->clients = g_array_new(TRUE, TRUE, sizeof(Client_t *));
  g_array_set_clear_func(data->clients, free_client);

  return data;
}

void free_data(RawData_t *data)
{
  g_array_free(data->clients, TRUE);

  g_free(data);
}

void data_add_client(RawData_t *data, Client_t *client)
{
  g_array_append_val(data->clients, client);
}
