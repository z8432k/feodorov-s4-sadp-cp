#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <glib.h>

#include "include/storage/json_store.h"
#include "include/storage/pgsql_store.h"

#define GETTEXT_PACKAGE "gtk20"
#include <glib/gi18n-lib.h>

typedef struct {
  gchar *number;
  gchar *model;
  gchar *color;
  gushort year;
  gboolean exists;
} SrcCar_t;

static SrcCar_t srcCar;
static GOptionEntry entries[] =
{
  { "number", 'n', 0, G_OPTION_ARG_STRING, &srcCar.number,
                "Государственный номер",
                "ANNNAA-NN" },

  { "model", 'm', 0, G_OPTION_ARG_STRING, &srcCar.model,
                "Модель",
                "Трабант" },

  { "color", 'c', 0, G_OPTION_ARG_STRING, &srcCar.color,
                "Цвет",
                "Ультрамарин" },

  { "year", 'y', 0, G_OPTION_ARG_INT, &srcCar.year,
                "Год выпуска",
                "1960" },

  { "exists", 'e', 0, G_OPTION_ARG_INT, &srcCar.exists,
                "Признак доступности",
                "1" },

  { NULL }
};

static inline void add_car(Car_t *car)
{
  RawData_t *data = load_data();

  data_add_car(data, car);

  save_data(data);
  save_data_pgsql(data);

  free_data(data);
}

int main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");

  GError *error = NULL;
  GOptionContext *context;

  context = g_option_context_new(" - Добавление автомобиля в базу");
  g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);

  if (!g_option_context_parse (context, &argc, &argv, &error)) {
    g_print ("option parsing failed: %s\n", error->message);
    exit (1);
  }

  Car_t *newCar = new_car();

  fill_car(newCar,
    srcCar.number,
    srcCar.model,
    srcCar.color,
    srcCar.year,
    srcCar.exists
  );

  add_car(newCar);
}

