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
gsize data_add_car(Car_t *car);
gssize data_drop_car(const gchar *number);
gssize data_drop_cars();
gssize data_service_car(const gchar *number, const gboolean flag);



void data_add_client(Client_t *client);

void data_truncate_clients();
void data_truncate_cars();

void data_drop_client(GString *license);

Client_t* data_search_client_by_license(Data_t *data, GString *license);
Client_t* data_search_client_by_request(Data_t *data, GString *request);

Car_t* data_search_car_by_number(Data_t *data, GString *number);
Car_t* data_search_car_by_request(Data_t *data, GString *request);

void data_car_to_service(Car_t *car, gboolean flag);
void data_rent_car(Car_t *car, Client_t *client, gboolean flag);

#endif //_DATA_H_
