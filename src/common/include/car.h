#include <glib.h>

typedef struct {
  GString *number;
  GString *model;
  GString *color;
  gushort year;
  gboolean exists;
} Car_t;

Car_t* new_car();

void free_car(gpointer car);

void fill_car(
  Car_t *car,
  const gchar *number,
  const gchar *model,
  const gchar *color,
  const gushort year,
  const gboolean exists
);
