#ifndef _STORAGE_DRIVER_H_
#define _STORAGE_DRIVER_H_

#include "data.h"

#define DRIVER_INIT_CB "driver_init"

typedef RawData_t* (*load_data_cb)();
typedef gssize (*add_car_cb)(const Car_t *);
typedef gssize (*drop_car_cb)(const gchar *number);
typedef gssize (*drop_cars_cb)();
typedef gssize (*service_car_cb)(const gchar *number, const gboolean flag);
typedef gssize (*add_client_cb)(const Client_t *client);
typedef gssize (*drop_client_cb)(const gchar *license);
typedef gssize (*drop_clients_cb)();
typedef gssize (*rent_car_cb)(const gchar *license, const gchar *number);
typedef gssize (*return_car_cb)(const gchar *license, const gchar *number);
typedef RawData_t* (*search_car_fragment_cb)(const gchar *reque);


typedef struct {
  load_data_cb impl_load_data_cb;
  add_car_cb impl_add_car_cb;
  drop_car_cb impl_drop_car_cb;
  drop_cars_cb impl_drop_cars_cb;
  service_car_cb impl_service_car_cb;
  add_client_cb impl_add_client_cb;
  drop_client_cb impl_drop_client_cb;
  drop_clients_cb impl_drop_clients_cb;
  rent_car_cb impl_rent_car_cb;
  return_car_cb impl_return_car_cb;
  search_car_fragment_cb impl_search_car_fragment_cb;
} DriverImpl;

typedef const DriverImpl* (* DriverInitCb)(void);

#define DRV_CALL(T, ...) ((driver->impl_##T)(__VA_ARGS__))

#endif //_STORAGE_DRIVER_H_
