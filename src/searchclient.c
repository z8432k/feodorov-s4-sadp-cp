#include <stdlib.h>
#include <locale.h>
#include <glib.h>
#include <string.h>

#include "rentcar.h"
#include "stringify_json.h"
#include "wordsearch.h"
#include "skiplist.h"

#define GETTEXT_PACKAGE "gtk20"

gchar *request;
gchar *license;

typedef struct {
    gchar *fragment;
    GPtrArray *box;
} ClientsLookupContext;

static GOptionEntry entries[] =
        {
                { "request", 'r', 0, G_OPTION_ARG_STRING, &request,
                        "Поисковый запрос по ФИО или адресу",
                        "пупкин" },

                { "license", 'l', 0, G_OPTION_ARG_STRING, &license,
                        "Поисковый запрос по номеру удостоверения",
                        "'78 AВ 123456'" },

                { NULL }
        };

const wchar_t *GetWC(const char *c)
{
  const size_t cSize = strlen(c)+1;
  wchar_t *wc = malloc(sizeof(wchar_t) * cSize);
  mbstowcs (wc, c, cSize);

  return wc;
}

gint lookup_clients(gpointer k, gpointer v, gpointer data)
{
 Client_t *client = v;
 ClientsLookupContext *context = data;

 const wchar_t *fragment = GetWC(context->fragment);
 const wchar_t *name = GetWC(client->name->str);;
 const wchar_t *addr = GetWC(client->address->str);;


  if (wordsearch(fragment, name) > -1) {
    g_ptr_array_add(context->box, client);
  }

  if (wordsearch(fragment, addr) > -1) {
    g_ptr_array_add(context->box, client);
  }

  free((void *) name);
  free((void *) addr);

 return 1;
}

typedef struct {
    GArray *cars;
    gchar *license;
} SearchContext;

void search_cars(gpointer data, gpointer user_data)
{
  RentRow_t *row = data;
  SearchContext *context = user_data;

  if (g_strcmp0(row->license->str, context->license) == 0) {
    g_array_append_val(context->cars, data);
  }
}

int main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");

  rentcar_init();

  GError *error = NULL;
  GOptionContext *context;

  context = g_option_context_new(" - Поиск клиента в базе");
  g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);

  g_option_context_set_description(context, "Поиск по номеру удостоверения имеет более высокий приоритет при указании обоих параметров одновременно.");

  if (!g_option_context_parse (context, &argc, &argv, &error)) {
      g_print ("option parsing failed: %s\n", error->message);
      exit (EXIT_FAILURE);
  }

  Data_t *data= structured_data();

    RawData_t *raw = data_new();

  GString *lic = g_string_new(license);

  if (license) {
    Client_t *client = avltree_lookup(data->clients, lic->str);

    if (!client) {
      g_printerr("Not found.\n");
      exit(EXIT_FAILURE);
    }

    g_array_append_val(raw->clients, client);

    SearchContext ctxt = {
        raw->cars,
        license
    };

    skiplist_foreach(data->rents, search_cars, &ctxt);

      gchar* clients = clients_stringify_json(raw);
      gchar* cars = cars_stringify_json(raw);

      g_print("%s", clients);
      g_print("%s", cars);
  }

  else if (request) {
    AVLTree *clients = data->clients;

    GPtrArray *result = g_ptr_array_new();

    ClientsLookupContext lookup_context = {
      request,
      result
    };

    avltree_foreach_pre(clients, lookup_clients, &lookup_context);

    gchar *json = clients_search_stringify_json(result);

    g_print("%s\n", json);

    g_ptr_array_free(result, TRUE);
  }
  else {
    g_printerr("Вы должны задать хотябы один поисковый запрос.\n");
  }
}

