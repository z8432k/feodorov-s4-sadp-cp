#include <jansson.h>

#include "data.h"

#define CLIENT_NAME "name"
#define CLIENT_LICENSE "license"
#define CLIENTS_PASSPORT "passport"
#define CLIENTS_ADDRESS "address"

#define CAR_NUMBER "number"
#define CAR_MODEL "model"
#define CAR_COLOR "color"
#define CAR_YEAR "year"
#define CAR_EXISTS "exists"

#define clients_array_to_json_array(data, builder) \
  data_array_to_json_array(data, (jsonFromData) builder)
#define cars_array_to_json_array(data, builder) \
  data_array_to_json_array(data, (jsonFromData) builder)

typedef json_t* (* jsonFromData)(void *);

json_t* data_array_to_json_array(GArray *array, jsonFromData builder);

gchar* clients_stringify_json(RawData_t *data);
gchar* cars_stringify_json(RawData_t *data);

gchar* cars_search_stringify_json(Car_t *car, Client_t *client);
gchar* clients_search_stringify_json(GPtrArray *clients);

json_t* json_build_client(Client_t *data);
json_t* json_build_car(Car_t *data);
