#include <stdlib.h>
#include <locale.h>
#include <glib.h>

#include "rentcar.h"

#define GETTEXT_PACKAGE "gtk20"

typedef struct {
  gchar *name;
  gchar *license;
  gchar *passport;
  gchar *address;
} SrcClient_t;

static SrcClient_t srcClient;
static GOptionEntry entries[] =
{
  { "name", 'n', 0, G_OPTION_ARG_STRING, &srcClient.name,
                "ФИО клиента",
                "Василий Батькович Пупкин" },

  { "license", 'l', 0, G_OPTION_ARG_STRING, &srcClient.license,
                "Номер водительского удостоверения в формате: 78 AВ 123456",
                "78тт123456" },

  { "passport", 'p', 0, G_OPTION_ARG_STRING, &srcClient.passport,
                "Паспортные данные",
                "123 258 Бла-бла-бла..." },

  { "address", 'a', 0, G_OPTION_ARG_STRING, &srcClient.address,
                "Адрес",
                "г.Можайск ул. Можайская дом 2 корпус 1 кв 3" },

  { NULL }
};

int main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");

  rentcar_init();

  GError *error = NULL;
  GOptionContext *context;

  context = g_option_context_new(" - Добавление клиента в базу");
  g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);

  if (!g_option_context_parse (context, &argc, &argv, &error)) {
    g_print ("option parsing failed: %s\n", error->message);
    exit (1);
  }

  if (!srcClient.name || !srcClient.license || !srcClient.passport || !srcClient.address) {
    g_printerr("Вы должны указать все необходимые данные для регистрации клиента.\n");
    exit(0);
  }

  Client_t *newClient = new_client();

  fill_client(newClient,
    srcClient.name,
    srcClient.license,
    srcClient.passport,
    srcClient.address
  );

  gsize code = data_add_client(newClient);

  free_client(newClient);

  if (code == 0) {
    exit(0);
  }
  else {
    exit(1);
  }
}

