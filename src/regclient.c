#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <glib.h>

#include "include/store.h"

#define GETTEXT_PACKAGE "gtk20"
#include <glib/gi18n-lib.h>

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
                "78 AВ 123456" },

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

  GError *error = NULL;
  GOptionContext *context;

  context = g_option_context_new(" - Добавление клиента в базу");
  g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);

  if (!g_option_context_parse (context, &argc, &argv, &error)) {
    g_print ("option parsing failed: %s\n", error->message);
    exit (1);
  }

  Client_t *newClient = new_client();

  g_string_assign(newClient->name, srcClient.name);
  g_string_assign(newClient->license, srcClient.license);
  g_string_assign(newClient->passport, srcClient.passport);
  g_string_assign(newClient->address, srcClient.address);

  reg_client(newClient);
}
