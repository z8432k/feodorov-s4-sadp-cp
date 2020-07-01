#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <glib.h>

#include "storage.h"

#define GETTEXT_PACKAGE "gtk20"

static gchar *number;
static gboolean exists;

static GOptionEntry entries[] =
{
  { "number", 'n', 0, G_OPTION_ARG_STRING, &number,
                "Государственный номер",
                "ANNNAA-NN" },

  { "service", 's', 0, G_OPTION_ARG_INT, &exists,
          "Признак ремонта",
          "1" },

  { NULL }
};

int main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");

  GError *error = NULL;
  GOptionContext *context;

  context = g_option_context_new(" - Отправка (возвращение) автомобиля на ремонт");
  g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);

  if (!g_option_context_parse (context, &argc, &argv, &error)) {
    g_print ("option parsing failed: %s\n", error->message);
    exit (1);
  }

  if (!number) {
    g_printerr("Вы должны указать как минимум номер автомобиля");
    exit(1);
  }

  data_service_car(number, !exists);
}

