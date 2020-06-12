#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <glib.h>

#include "include/jsonstore.h"

#define GETTEXT_PACKAGE "gtk20"

typedef struct {
    gchar *number;
    gchar *license;
    gboolean rent;
} InputData_t;

static InputData_t inputData;

static GOptionEntry entries[] =
        {
                { "number", 'n', 0, G_OPTION_ARG_STRING, &inputData.number,
                        "Государственный номер",
                        "ANNNAA-NN" },

                { "license", 'l', 0, G_OPTION_ARG_STRING, &inputData.license,
                        "Номер водительского удостоверения клиента",
                        "'78 AВ 123456'" },

                { "rent", 'r', 0, G_OPTION_ARG_INT, &inputData.rent,
                        "Признак аренды",
                        "1" },

                { NULL }

        };

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");

    GError *error = NULL;
    GOptionContext *context;

    context = g_option_context_new(" - Выдача (возвращение) автомобиля в прокат");
    g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);

    if (!g_option_context_parse (context, &argc, &argv, &error)) {
        g_print ("option parsing failed: %s\n", error->message);
        exit (1);
    }

    printf("Not implemented.\n");
}

