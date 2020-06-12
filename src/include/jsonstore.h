#include <glib.h>

#include "data.h"

RawData_t* load_data();
void save_data(RawData_t *data);

gchar* clients_stringify(RawData_t *data);
gchar* cars_stringify(RawData_t *data);
