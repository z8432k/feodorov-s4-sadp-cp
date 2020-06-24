#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <glib.h>

#include "storage/include/json_store.h"

#define GETTEXT_PACKAGE "gtk20"

gchar *request;
gchar *number;

static GOptionEntry entries[] =
        {
                { "request", 'r', 0, G_OPTION_ARG_STRING, &request,
                        "Поисковый запрос по цвету автомобиля",
                        "голубой" },

                { "number", 'n', 0, G_OPTION_ARG_STRING, &number,
                        "Поисковый запрос по гос. номеру",
                        "ANNNAA-NN" },

                { NULL }
        };

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");

    GError *error = NULL;
    GOptionContext *context;

    context = g_option_context_new(" - Поиск автомобиля в базе");
    g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);

    g_option_context_set_description(context, "Поиск по гос. номеру имеет более высокий приоритет при указании обоих параметров одновременно.");

    if (!g_option_context_parse (context, &argc, &argv, &error)) {
        g_print ("option parsing failed: %s\n", error->message);
        exit (1);
    }

    Data_t *data= structured_data();


    GString *key = g_string_new(number);
    if (number) {
      Car_t *car = (Car_t *) hash_tab_get(data->cars, key);

      RentRow_t *search_row = new_rent_row();

      g_string_printf(search_row->number, "%s", car->number->str);

      SListItem *row = skiplist_find(data->rents, search_row);

      // g_print("%s", ((RentRow_t *) row->data)->license->str);
      GString *license = ((RentRow_t *) row->data)->license;

      Client_t *client = avltree_lookup(data->clients, license->str);

      g_print("%s", client->name->str);
    }
    else if (request) {
      g_printerr("Not implemented.\n");
    }
    else {
      g_printerr("Вы должны задать хотябы один поисковый запрос.\n");
    }

}

