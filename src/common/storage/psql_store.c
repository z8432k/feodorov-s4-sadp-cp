#include "pgsql_store.h"

static const gchar *uri = "postgresql://localhost/rentcar";

PGconn *connection;

PGconn* pgGetConnection()
{
  if (connection == NULL) {
      connection = PQconnectdb(uri);
  }

  return connection;
}

void save_data_pgsql(RawData_t *data)
{
  pgGetConnection();
  printf("Connect.\n");
}
