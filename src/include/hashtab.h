// #ifndef _HASHTAB_H_
// #define _HASHTAB_H_
// #endif //_HASHTAB_H_

#include <glib.h>

typedef struct {
  gsize size;
  GArray *data;
} HashTab_t;

HashTab_t* hash_tab_new();
void hash_tab_free(HashTab_t *tab);

gssize hash_tab_set(HashTab_t *tab, GString *key, gconstpointer data);
gssize hash_tab_remove(HashTab_t *tab, GString *key);
gconstpointer hash_tab_get(HashTab_t *tab, GString *key);