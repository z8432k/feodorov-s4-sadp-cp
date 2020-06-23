#ifndef _DATA_H_
#define _DATA_H_

#include <glib.h>

#include "client.h"
#include "car.h"

typedef struct {
    GTree *clients;
} Data_t;

typedef struct {
    GArray *clients;  // GArray<Client_t>[]
    GArray *cars;     // GArray<Car_t>[]
} RawData_t;

typedef void* (* dataSearchFunc_t)(Data_t *data, GString *license);

RawData_t* new_data();
RawData_t* data_load();
void free_data(RawData_t *data);
gssize data_add_car(const Car_t *car);
gssize data_drop_car(const gchar *number);
gssize data_drop_cars();
gssize data_service_car(const gchar *number, const gboolean flag);
gssize data_add_client(const Client_t *client);
gssize data_drop_client(const gchar *license);
gssize data_drop_clients();




#endif //_DATA_H_
