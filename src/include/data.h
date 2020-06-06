#include <glib.h>

typedef struct {
  GTree *clients;
} Data_t;

typedef struct {
  GArray *clients; // GArray<Client_t>[]
} RawData_t;

RawData_t* new_data();
void free_data(RawData_t *data);

void data_add_client(RawData_t *data, Client_t *client);

