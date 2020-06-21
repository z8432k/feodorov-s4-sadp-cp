#include "sqlite3.h"

#include "../../include/data.h"

RawData_t* load_data_sqlite();
void save_data_sqlite(RawData_t *data);