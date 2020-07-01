#include <stdlib.h>
#include <locale.h>
#include <glib.h>
#include "string.h"

#include "rentcar.h"
#include "stringify_json.h"

#define GETTEXT_PACKAGE "gtk20"

gchar *request;
gchar *number;

static GOptionEntry entries[] = {
    { "request", 'r', 0, G_OPTION_ARG_STRING, &request,
            "Поисковый запрос по цвету автомобиля",
            "голубой" },

    { "number", 'n', 0, G_OPTION_ARG_STRING, &number,
            "Поисковый запрос по гос. номеру",
            "ANNNAA-NN" },

    { NULL }
  };

const wchar_t *GetWC(const char *c)
{
  const size_t cSize = strlen(c)+1;
  wchar_t *wc = malloc(sizeof(wchar_t) * cSize);
  mbstowcs (wc, c, cSize);

  return wc;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");

    rentcar_init();

    GError *error = NULL;
    GOptionContext *context;

    context = g_option_context_new(" - Поиск автомобиля в базе");
    g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);

    g_option_context_set_description(context, "Поиск по гос. номеру имеет более высокий приоритет при указании обоих параметров одновременно.");

    if (!g_option_context_parse (context, &argc, &argv, &error)) {
        g_print ("option parsing failed: %s\n", error->message);

        exit(EXIT_FAILURE);
    }

    Data_t *data = structured_data();


    GString *key = g_string_new(number);
    if (number) {
      Car_t *car = (Car_t *) hash_tab_get(data->cars, key);

      RentRow_t *search_row = new_rent_row();

      if (!car) {
        g_printerr("Not found.");

        exit(EXIT_FAILURE);
      }

      g_string_printf(search_row->number, "%s", car->number->str);

      SListItem *row = skiplist_find(data->rents, search_row);

      // g_print("%s", ((RentRow_t *) row->data)->license->str);

        Client_t *client = NULL;
        if (row) {
          GString *license = ((RentRow_t *) row->data)->license;
            client = avltree_lookup(data->clients, license->str);
        }


      gchar *result = cars_search_stringify_json(car, client);

      g_print("%s", result);
    }
    else if (request) {
      // RawData_t* result = data_search_car_fragment(request);

      g_print("Not implemented yet.\n");
    }
    else {
      g_printerr("Вы должны задать хотябы один поисковый запрос.\n");
    }

}

