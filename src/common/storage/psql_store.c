#include <stdlib.h>
#include <glib.h>
#include "psql_store.h"
#include <errno.h>


// CARS
static gchar *select_cars_sql = "SELECT * FROM cars;";
static gchar *select_car_sql = "SELECT * FROM cars WHERE car_number = $1;";
static gchar *insert_car_sql = "INSERT INTO cars (car_number, model, color, year) VALUES ($1, $2, $3, $4);";
static gchar *delete_cars_sql = "DELETE from cars WHERE 1 = 1;";
static gchar *delete_car_sql = "DELETE FROM cars WHERE car_number = $1 RETURNING *;";
static gchar *service_car_sql = "UPDATE cars SET exi = $2 WHERE car_number = $1;";

// CLIENTS
static gchar *insert_client_sql = "INSERT INTO clients (client_name, license, passport, address) VALUES ($1, $2, $3, $4);";
static gchar *select_clients_sql = "SELECT * from clients;";
static gchar *delete_clients_sql = "DELETE from clients WHERE 1 = 1;";
static gchar *delete_client_sql = "DELETE FROM clients WHERE license = $1 RETURNING *;";

// RENT
static gchar *select_rents_sql = "SELECT * FROM rents;";
static gchar *insert_rent_sql = "INSERT INTO rents (client_license, car_number, rent_date) VALUES ($1, $2, now())";
static gchar *update_rent_sql = "UPDATE rents SET return_date = now() WHERE client_license = $1 AND car_number = $2;";

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

gssize add_car_impl(const Car_t *car)
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

RawData_t* load_data_impl()
{
  PGconn *conn = pgGetConnection();

  RawData_t *data = new_data();

  // load cars
  res = PQexec(conn, select_cars_sql);

  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    gsize rows = PQntuples(res);

    Car_t *car;
    for (gsize i = 0; i < rows; i++) {
      car = new_car();

      fill_car(car,
        PQgetvalue(res, i, 1),
        PQgetvalue(res, i, 2),
        PQgetvalue(res, i, 3),
        atoi(PQgetvalue(res, i, 4)),
        to_bool(PQgetvalue(res, i, 4))
      );

      g_array_append_val(data->cars, car);
    }
  }
  else {
    pg_error_exit();
  }

  // load cars
  res = PQexec(conn, select_clients_sql);

  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    gsize rows = PQntuples(res);

    Client_t *client;
    for (gsize i = 0; i < rows; i++) {
      client = new_client();

      fill_client(client,
         PQgetvalue(res, i, 1),
         PQgetvalue(res, i, 2),
         PQgetvalue(res, i, 3),
         PQgetvalue(res, i, 4)
      );

      g_array_append_val(data->clients, client);
    }
  }
  else {
    pg_error_exit();
  }

  // load rents
  res = PQexec(conn, select_rents_sql);

  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
      gsize rows = PQntuples(res);

      RentRow_t *row;
      for (gsize i = 0; i < rows; i++) {
          row = new_rent_row();

          fill_rent_row(row,
              PQgetvalue(res, i, 1),
              PQgetvalue(res, i, 2),
              PQgetvalue(res, i, 3),
              PQgetvalue(res, i, 4)
          );

          g_array_append_val(data->rents, row);
        }

      pg_normal_exit();
    }
  else {
      pg_error_exit();
    }

  return data;
}

gssize drop_cars_impl()
{
  PGconn *conn = pgGetConnection();

  res = PQexec(conn, delete_cars_sql);

  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    pg_error_exit();

    return -1;
  }
  else {
    pg_normal_exit();

    return 0;
  }
}

gssize drop_car_impl(const gchar *number)
{
  PGconn *conn = pgGetConnection();

  res = PQexecParams(conn, delete_car_sql,
                     1, NULL, &number, NULL, NULL, 0);

  gsize affected = PQntuples(res);

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
      pg_error_exit();
      return -1;
    }
  else {
    pg_normal_exit();

    if (affected > 0) {
      return 0;
    }

    errno = ENOENT;
    perror("");
    return 1;
  }
}

gssize service_car_impl(const gchar *number, const gboolean flag)
{
  PGconn *conn = pgGetConnection();

  const gchar *params[2] = {
      number,
      flag ? "t" : "f"
  };

  res = PQexecParams(conn, service_car_sql,
                     2, NULL, params, NULL, NULL, 0);

  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    pg_error_exit();

    return -1;
  }
  else {
    pg_normal_exit();

    return 0;
  }
}

gssize add_client_impl(const Client_t *client)
{
  const gchar *params[4] = {
      client->name->str,
      client->license->str,
      client->passport->str,
      client->address->str
  };

  PGconn *conn = pgGetConnection();

  res = PQexecParams(conn, insert_client_sql, 4, NULL, params, NULL, NULL, 0);

  if (PQresultStatus(res) == PGRES_COMMAND_OK) {
    pg_normal_exit();
  }
  else {
    pg_error_exit();

    return -1;
  }

  return 0;
}

gssize drop_client_impl(const gchar *license)
{
  PGconn *conn = pgGetConnection();

  res = PQexecParams(conn, delete_client_sql,
                     1, NULL, &license, NULL, NULL, 0);

  gsize affected = PQntuples(res);

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
      pg_error_exit();
      return -1;
    }
  else {
      pg_normal_exit();

      if (affected > 0) {
          return 0;
        }

      errno = ENOENT;
      perror("");
      return 1;
    }
}

gssize drop_clients_impl()
{
  PGconn *conn = pgGetConnection();

  res = PQexec(conn, delete_clients_sql);

  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
      pg_error_exit();

      return -1;
    }
  else {
      pg_normal_exit();

      return 0;
    }
}

gssize rent_car_impl(const gchar *license, const gchar *number)
{
  const gchar *params[2] = {
    license,
    number
  };

  PGconn *conn = pgGetConnection();

  res = PQexecParams(conn, insert_rent_sql, 2, NULL, params, NULL, NULL, 0);

  if (PQresultStatus(res) == PGRES_COMMAND_OK) {
    pg_normal_exit();
  }
  else {
    pg_error_exit();

    return -1;
  }

  return 0;
}

gssize return_car_impl(const gchar *license, const gchar *number)
{
  const gchar *params[2] = {
      license,
      number
  };

  PGconn *conn = pgGetConnection();

  res = PQexecParams(conn, update_rent_sql, 2, NULL, params, NULL, NULL, 0);

  if (PQresultStatus(res) == PGRES_COMMAND_OK) {
      pg_normal_exit();
    }
  else {
      pg_error_exit();

      return -1;
    }

  return 0;
}
