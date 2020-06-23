#include <stdlib.h>
#include <glib.h>
#include "psql_store.h"

static const gchar *uri = "postgresql://localhost/rentcar";

// CARS
static gchar *select_cars_sql = "SELECT * FROM CARS;";
static gchar *select_car_sql = "SELECT * FROM cars WHERE car_number = $1;";
static gchar *insert_car_sql = "INSERT INTO cars (car_number, model, color, year) VALUES ($1, $2, $3, $4);";
static gchar *truncate_cars_sql = "TRUNCATE TABLE cars;";
static gchar *delete_car_sql = "DELETE FROM cars WHERE car_number = $1;";
static gchar *service_car_sql = "UPDATE cars SET exi = $1 WHERE car_number = $2;";


static gchar *all_clients_sql = "select * from clients;";
static gchar *all_rent_rows_sql = "select * from rents;";

static PGconn *connection;

static PGresult *res;

void do_exit(PGconn *conn) {
  PQfinish(conn);
  exit(1);
}

void pg_normal_exit() {
  if(res != NULL) {
    PQclear(res);
  }

  if(connection != NULL) {
    PQfinish (connection);
  }
}

void pg_error_exit() {
  g_printerr("%s", PQerrorMessage(connection));

  if(res != NULL) {
      PQclear(res);
    }

  if(connection != NULL) {
    PQfinish (connection);
  }
}

static void terminate(int code)
{
  if(code != 0)
    g_printerr("%s\n", PQerrorMessage(connection));

  if(res != NULL)
    PQclear(res);

  if(connection != NULL)
    PQfinish(connection);

  exit(code);
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

  res = PQexec(conn, select_cars_sql);

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
      PQgetvalue(res, i, 2),
      PQgetvalue(res, i, 3),
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

gssize add_car_impl(Car_t *car)
{
  GString *year = g_string_new("");
  g_string_printf(year, "%d", car->year);

  const gchar *params[4] = {
      car->number->str,
      car->model->str,
      car->color->str,
      year->str
  };

  PGconn *conn = pgGetConnection();

  res = PQexecParams(conn, insert_car_sql, 4, NULL, params, NULL, NULL, 0);

  if (PQresultStatus(res) == PGRES_COMMAND_OK) {
    pg_normal_exit();
  }
  else {
    pg_error_exit();
  }

  g_string_free(year, TRUE);

  return 0;
}

gssize get_car_impl(Car_t *car) {
  PGconn *conn = pgGetConnection();

  const gchar *number = car->number->str;

  res = PQexecParams(conn, select_car_sql,
      1, NULL, &number, NULL, NULL, 0);

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    g_printerr("Car does not exist.\n");
    PQclear(res);
    do_exit(conn);
  }
  else {
    g_string_printf(car->number, "%s", PQgetvalue(res, 0, 1));
    g_string_printf(car->model, "%s", PQgetvalue(res, 0, 3));
    g_string_printf(car->color, "%s", PQgetvalue(res, 0, 2));
    car->year = atoi(PQgetvalue(res, 0, 4));
    car->exists = to_bool(PQgetvalue(res, 0, 5));
  }

  return 0;
}
