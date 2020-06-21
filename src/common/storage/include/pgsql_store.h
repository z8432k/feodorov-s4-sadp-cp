#include <postgresql/libpq-fe.h>

#include "../../include/data.h"

RawData_t* load_data_pgsql();
void save_data_pgsql(RawData_t *data);