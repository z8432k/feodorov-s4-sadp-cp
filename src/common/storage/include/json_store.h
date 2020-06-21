#include "jansson.h"

#include "../../include/data.h"

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

json_t* json_build_client(Client_t *data);
json_t* json_build_car(Car_t *data);

RawData_t* load_data();
void save_data(RawData_t *data);

