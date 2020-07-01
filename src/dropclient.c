#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <glib.h>

#include "rentcar.h"

#define GETTEXT_PACKAGE "gtk20"
#include <glib/gi18n-lib.h>

static gchar *license;
static gboolean all;

static GOptionEntry entries[] =
{
  { "all", 'a', 0, G_OPTION_ARG_NONE, &all,
                "Удалить всех клиентов",
                NULL },

  { "license", 'l', 0, G_OPTION_ARG_STRING, &license,
      "Удалить клиента по номеру водительского удостоверения",
      NULL },

  { NULL }
};

int main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");

  rentcar_init();

  GError *error = NULL;
  GOptionContext *context;

  context = g_option_context_new(" - Удаление клиентов");
  g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);

  if (!g_option_context_parse (context, &argc, &argv, &error)) {
    g_print ("option parsing failed: %s\n", error->message);
    exit (1);
  }

  gssize code;

  if (license) {
    code = data_drop_client(license);
  }
  else if (all) {
    code = data_drop_clients();
  }
  else {
    g_printerr("Вы должны указать номер удостоверения или установить флаг удаления всех клиентов.\n");
    exit(1);
  }

  if (code < 0) {
    exit(1);
  }
  else {
    exit(0);
  }
}

