#include "storage.h"
#include "data.h"
#include "storage_driver.h"
#include <dlfcn.h>
#include <glib.h>
#include <stdlib.h>

#define DRIVER_PATH "librentcar_pgsql_store.so"

static gpointer driver_object;
static const DriverImpl *driver;

gint storage_init() {
  gint result = 0;

  driver_object = dlopen(DRIVER_PATH, RTLD_NOW);

  // TODO: Graceful exit
  if (!driver_object) {
    g_printerr("%s\n", dlerror());

    exit(EXIT_FAILURE);
  }

  DriverInitCb driver_init = (DriverInitCb) dlsym(driver_object, DRIVER_INIT_CB);

  driver = driver_init();

  if (!driver) {
    g_printerr("%s\n", dlerror());

    exit(EXIT_FAILURE);
  }

  return result;
}

RawData_t* data_load() {
  return DRV_CALL(load_data_cb);
}

gssize data_add_client(const Client_t *client)
{
  return DRV_CALL(add_client_cb, client);
}

gssize data_add_car(const Car_t *car)
{
  return DRV_CALL(add_car_cb, car);
}

gssize data_drop_cars()
{
  return DRV_CALL(drop_cars_cb);
}

gssize data_drop_car(const gchar *number)
{
  return DRV_CALL(drop_car_cb, number);
}

gssize data_service_car(const gchar *number, const gboolean flag)
{
  return DRV_CALL(service_car_cb, number, flag);
}

gssize data_drop_client(const gchar *license)
{
  return DRV_CALL(drop_client_cb, license);
}

gssize data_drop_clients()
{
  return DRV_CALL(drop_clients_cb);
}

gssize data_rent_car(const gchar *license, const gchar *number)
{
  return DRV_CALL(rent_car_cb, license, number);
}

gssize data_return_car(const gchar *license, const gchar *number)
{
  return DRV_CALL(return_car_cb, license, number);
}


RawData_t* data_search_car_fragment(const gchar *request)
{
  return DRV_CALL(search_car_fragment_cb, request);
}
