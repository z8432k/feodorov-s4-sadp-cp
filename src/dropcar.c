#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <glib.h>

#include "include/jsonstore.h"

#define GETTEXT_PACKAGE "gtk20"
#include <glib/gi18n-lib.h>

static gboolean all;

static GOptionEntry entries[] =
{
  { "all", 'a', 0, G_OPTION_ARG_NONE, &all,
                "Удалить все автомобили",
                NULL },

  { NULL }
};

int main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");

  GError *error = NULL;
  GOptionContext *context;

  context = g_option_context_new(" - Удаление автомобилей");
  g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);

  if (!g_option_context_parse (context, &argc, &argv, &error)) {
    g_print ("option parsing failed: %s\n", error->message);
    exit (1);
  }

  if (all) {
    RawData_t *data = load_data();

    data_truncate_cars(data);

    save_data(data);
  }
  else {
    printf("Not impolemented.\n");
  }
}

