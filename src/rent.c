#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <glib.h>

#include "rentcar.h"

#define GETTEXT_PACKAGE "gtk20"

static gchar *number;
static gchar *license;
static gboolean rent;

static GOptionEntry entries[] =
        {
                { "number", 'n', 0, G_OPTION_ARG_STRING, &number,
                        "Государственный номер",
                        "ANNNAA-NN" },

                { "license", 'l', 0, G_OPTION_ARG_STRING, &license,
                        "Номер водительского удостоверения клиента",
                        "'78 AВ 123456'" },

                { "rent", 'r', 0, G_OPTION_ARG_INT, &rent,
                        "Признак аренды",
                        "1" },

                { NULL }

        };

int main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");

  rentcar_init();

  GError *error = NULL;
  GOptionContext *context;

  context = g_option_context_new(" - Выдача (возвращение) автомобиля в прокат");
  g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);

  if (!g_option_context_parse (context, &argc, &argv, &error)) {
      g_print ("option parsing failed: %s\n", error->message);
      exit (1);
  }

  if (!license || !number) {
    g_printerr("Вы должны указать удостоверение и номер автомобиля.\n");
  }

  gsize code;
  if (rent) {
    code = data_rent_car(license, number);
  }
  else {
    code = data_return_car(license, number);
  }

  if (code == 0) {
    exit(0);
  }
  else {
    exit(1);
  }
}

