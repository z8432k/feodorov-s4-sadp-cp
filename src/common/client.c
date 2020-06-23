#include "client.h"

Client_t* new_client()
{
  Client_t *client = g_new(Client_t, 1);

  client->name = g_string_new(NULL);
  client->license = g_string_new(NULL);
  client->passport = g_string_new(NULL);
  client->address = g_string_new(NULL);

  return client;
}

void free_client(Client_t *client)
{

  g_string_free(client->name, TRUE);
  g_string_free(client->license, TRUE);
  g_string_free(client->passport, TRUE);
  g_string_free(client->address, TRUE);

  g_free(client);
}

void fill_client(Client_t *client, const gchar *name, const gchar *license, const gchar *passport, const gchar *address)
{
  g_string_assign(client->name, name);
  g_string_assign(client->license, license);
  g_string_assign(client->passport, passport);
  g_string_assign(client->address, address);
}
