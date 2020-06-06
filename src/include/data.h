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

RawData_t* new_data();
void free_data(RawData_t *data);

void data_add_client(RawData_t *data, Client_t *client);
void data_add_car(RawData_t *data, Car_t *car);

