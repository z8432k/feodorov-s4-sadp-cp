//
// Created by alexander on 28.06.2020.
//

#ifndef _PLUGIN_STORAGE_H_
#define _PLUGIN_STORAGE_H_

#include "data.h"

gssize add_car_impl(const Car_t *car);
RawData_t* load_data_impl();
gssize drop_cars_impl();
gssize drop_car_impl(const gchar *number);
gssize service_car_impl(const gchar *number, const gboolean flag);
gssize add_client_impl(const Client_t *client);
gssize drop_client_impl(const gchar *license);
gssize drop_clients_impl();
gssize rent_car_impl(const gchar *license, const gchar *number);
gssize return_car_impl(const gchar *license, const gchar *number);
RawData_t *search_car_fragment_impl(const gchar *request);

#endif //_PLUGIN_STORAGE_H_
