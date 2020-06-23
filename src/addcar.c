#include <stdlib.h>
#include <locale.h>
#include <glib.h>
#include "data.h"

#define GETTEXT_PACKAGE "gtk20"

typedef struct {
  gchar *number;
  gchar *model;
  gchar *color;
  gushort year;
} SrcCar_t;

static SrcCar_t srcCar;
static GOptionEntry entries[] =
{
  { "number", 'n', 0, G_OPTION_ARG_STRING, &srcCar.number,
                "Государственный номер",
                "ANNNAANN" },

  { "model", 'm', 0, G_OPTION_ARG_STRING, &srcCar.model,
                "Модель",
                "\"Трабант\"" },

  { "color", 'c', 0, G_OPTION_ARG_STRING, &srcCar.color,
                "Цвет",
                "\"Ультрамарин\"" },

  { "year", 'y', 0, G_OPTION_ARG_INT, &srcCar.year,
                "Год выпуска",
                "1960" },

  { NULL }
};

static void check_input()
{
  if (!srcCar.number || !srcCar.model || !srcCar.color || !srcCar.year) {
    g_printerr("Вы должны указать все поля для вновь добавляемой записи автомобиля.\n");
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");

  GError *error = NULL;
  GOptionContext *context;

  context = g_option_context_new(" - Добавление автомобиля в базу");
  g_option_context_add_main_entries(context, entries, GETTEXT_PACKAGE);

  if (!g_option_context_parse(context, &argc, &argv, &error)) {
    g_print ("option parsing failed: %s\n", error->message);
    exit (1);
  }

  check_input();

  Car_t *newCar = new_car();

  fill_car(newCar,
    srcCar.number,
    srcCar.model,
    srcCar.color,
    srcCar.year,
    FALSE
  );


  gssize code = data_add_car(newCar);

  free_car(newCar);

  if (code == 0) {
    exit(0);
  }

  exit(1);
}

