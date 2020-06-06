#include <glib.h>

typedef struct {
  GString *name;
  GString *license;
  GString *passport;
  GString *address;
} Client_t;

typedef struct {
  GTree *clients;
} Data_t;

typedef struct {
  GArray *clients; // GArray<Client_t>[]
} RawData_t;

Client_t* new_client();
void free_client(gpointer client);

RawData_t* new_raw_data();
void free_raw_data(RawData_t *data);

void data_add_client(RawData_t *data, Client_t *client);

void reg_client(Client_t *client);
