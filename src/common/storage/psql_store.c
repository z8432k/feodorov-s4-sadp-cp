#include <stdlib.h>
#include "pgsql_store.h"

static const gchar *uri = "postgresql://localhost/rentcar";

static gchar *all_cars_sql = "select * from cars;";
static gchar *all_clients_sql = "select * from clients;";
static gchar *all_rent_rows_sql = "selsect * from rents;";

PGconn *connection;

void do_exit(PGconn *conn) {
  PQfinish(conn);
  exit(1);
}

static gboolean  to_bool(gchar *str) {
  return g_strcmp0(str, "t") == 0 ? TRUE : FALSE;
}

PGconn* pgGetConnection()
{
  if (connection == NULL) {
      connection = PQconnectdb("user=alexander dbname=rentcar");;

    if (PQstatus(connection) == CONNECTION_BAD) {

      g_printerr("Connection to database failed: %s\n", PQerrorMessage(connection));
      do_exit(connection);
    }
  }

  return connection;
}

void save_data_pgsql(RawData_t *data)
{
  pgGetConnection();
  printf("Connect.\n");
}

RawData_t* load_data_pgsql()
{
  RawData_t *data = new_data();

  PGconn *conn = pgGetConnection();

  PGresult *res = PQexec(conn, all_cars_sql);

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {

    g_print("No data retrieved\n");
    PQclear(res);
    do_exit(conn);
  }

  gsize rows = PQntuples(res);

  Car_t *car;
  gchar *tr;
  for (gsize i = 0; i < rows; i++) {
    car = new_car();
    tr = PQgetvalue(res, i, 5);
    fill_car(car,
      PQgetvalue(res, i, 1),
      PQgetvalue(res, i, 3),
      PQgetvalue(res, i, 2),
      atoi(PQgetvalue(res, i, 4)),
      to_bool(PQgetvalue(res, i, 5))
    );
  }

  PQclear(res);

  res = PQexec(conn, all_clients_sql);

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    g_print("No data retrieved\n");
    PQclear(res);
    do_exit(conn);
  }

  rows = PQntuples(res);

  Client_t *client;
  for (gsize i = 0; i < rows; i++) {
    client = new_client();
    tr = PQgetvalue(res, i, 5);
    fill_client(client,
      PQgetvalue(res, i, 1),
      PQgetvalue(res, i, 2),
      PQgetvalue(res, i, 3),
      PQgetvalue(res, i, 4)
    );
  }

  PQfinish(conn);

  return 0;
}
