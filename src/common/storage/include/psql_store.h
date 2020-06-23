#include <postgresql/libpq-fe.h>

#include "data.h"

gssize add_car_impl(Car_t *car);
RawData_t* load_data_impl();
gssize drop_cars_impl();
gssize drop_car_impl(const gchar *number);
gssize service_car_impl(const gchar *number, const gboolean flag);
