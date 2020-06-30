#ifndef _DATA_H_
#define _DATA_H_

#include <glib.h>

#include "data_types.h"
#include "avltree.h"
#include "hashtab.h"
#include "skiplist.h"

typedef struct {
    AVLTree *clients;
    HashTab_t *cars;
    SList *rents;
} Data_t;

typedef struct {
    GArray *clients;  // GArray<Client_t>[]
    GArray *cars;     // GArray<Car_t>[]
    GArray *rents;    // GArray<RentRow_t>[]
} RawData_t;

typedef void* (* dataSearchFunc_t)(Data_t *data, GString *license);

RawData_t* data_new();
void data_free(RawData_t *data);

Data_t* structured_data();
void structured_data_free(Data_t *data);


#endif //_DATA_H_
