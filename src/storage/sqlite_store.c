#include <stdlib.h>
#include <stdio.h>

#include "../include/storage/sqlite_store.h"

#define SQLITE_DATA_FILENAME "car_rent_data.sqlite3"

#define SQLITE_(call)                           \
  {                                             \
    gint result = (call);                       \
    if (result == SQLITE_OK) {                  \
    }                                           \
    else {                                      \
    fprintf(stderr, "%s", sqlite3_errmsg(db));  \
      sqlite3_close(db);                        \
      exit(1);                                  \
    }                                           \
  }

RawData_t* load_data_sqlite()
{
  RawData_t *data = new_data();

  sqlite3 *db;
  SQLITE_(sqlite3_open(SQLITE_DATA_FILENAME, &db))

  return data;
}