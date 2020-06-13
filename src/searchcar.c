#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <glib.h>

#include "include/json_store.h"

#define GETTEXT_PACKAGE "gtk20"

typedef struct {
    gchar *request;
    gchar *number;
} InputData_t;

static InputData_t inputData;

static GOptionEntry entries[] =
        {
                { "request", 'r', 0, G_OPTION_ARG_STRING, &inputData.request,
                        "Поисковый запрос по цвету автомобиля",
                        "голубой" },

                { "number", 'n', 0, G_OPTION_ARG_STRING, &inputData.number,
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

    printf("Not implemented.\n");
}

