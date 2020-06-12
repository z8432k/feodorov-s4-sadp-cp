#include <glib.h>

typedef struct {
    GString *license;
    GString *number;
    GString *rentdate;
    GString *returndate;
} RentRow_t;

RentRow_t* new_rent_row();

void free_rent_row(RentRow_t *row);

void fill_rent_row(
        RentRow_t *row,
        const gchar *license,
        const gchar *number,
        const gchar *rentdate,
        const gchar *returndate
);
