#include "car.h"

Car_t* new_car()
{
  Car_t *car = g_new(Car_t, 1);

  car->number = g_string_new(NULL);
  car->model = g_string_new(NULL);
  car->color = g_string_new(NULL);
  car->year = 0;
  car->exists = FALSE;

  return car;
}

void free_car(gpointer data)
{
  Car_t **car = (Car_t **) data;

  g_string_free((*car)->number, TRUE);
  g_string_free((*car)->color, TRUE);
  g_string_free((*car)->model, TRUE);

  g_free(*car);
}

void fill_car(Car_t *car, const gchar *number, const gchar *color, const gchar *model, const gushort year, const gboolean exists)
{
  g_string_assign(car->number, number);
  g_string_assign(car->model, model);
  g_string_assign(car->color, color);
  car->year = year;
  car->exists = exists;
}
