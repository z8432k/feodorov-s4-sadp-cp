#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <glib.h>

#include "rentcar.h"

#define GETTEXT_PACKAGE "gtk20"
#include <glib/gi18n-lib.h>

static gboolean all;
static gchar *number;

static GOptionEntry entries[] =
{
  { "all", 'a', 0, G_OPTION_ARG_NONE, &all,
                "Удалить все автомобили",
                NULL },

  { "number", 'n', 0, G_OPTION_ARG_STRING, &number,
      "Удалить автомобиль по номеру",
      NULL },

  { NULL }
};

int main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");

  rentcar_init();

  GError *error = NULL;
  GOptionContext *context;

  context = g_option_context_new(" - Удаление автомобилей");
  g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);

  if (!g_option_context_parse (context, &argc, &argv, &error)) {
    g_print ("option parsing failed: %s\n", error->message);
    exit (1);
  }

  gssize code;

  if (number) {
    code = data_drop_car(number);
  }
  else if (all) {
    code = data_drop_cars();
  }
  else {
    g_printerr("Вы должны указать номер автомобиля или установить флаг удаления всех автомобилей.\n");
    exit(1);
  }

  if (code < 0) {
    exit(1);
  }
  else {
    exit(0);
  }
}

