#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <glib.h>
#include "data.h"

RawData_t* data_load();
gssize data_add_car(const Car_t *car);
gssize data_drop_car(const gchar *number);
gssize data_drop_cars();
gssize data_service_car(const gchar *number, const gboolean flag);
gssize data_add_client(const Client_t *client);
gssize data_drop_client(const gchar *license);
gssize data_drop_clients();
gssize data_rent_car(const gchar *license, const gchar *number);
gssize data_return_car(const gchar *license, const gchar *number);
RawData_t* data_search_car_fragment(const gchar *request);

#endif //_STORAGE_H_
