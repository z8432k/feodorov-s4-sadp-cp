#include <glib.h>

typedef struct {
  GString *name;
  GString *license;
  GString *passport;
  GString *address;
} Client_t;

Client_t* new_client();

void free_client(gpointer client);

void fill_client(
  Client_t *client,
  const gchar *name,
  const gchar *license,
  const gchar *passport,
  const gchar *address
);
