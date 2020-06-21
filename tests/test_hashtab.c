#include <glib.h>

#include "hashtab.h"

static int arr[] = {1,2,3,4};
static gchar *keys[] = {
    "AAAA0ILKJ",
    "ACA0A12",
    "AA0AA55",
    "A0AAN67"
};

int main(void)
{
  HashTab_t *tab = hash_tab_new();

  GString *key;
  for (gsize i = 0; i < 4; i++) {
    key = g_string_new(keys[i]);
    hash_tab_set(tab, key, &arr[i]);
  }

  for (gsize i = 0; i < 4; i++) {
    key = g_string_new(keys[i]);
    int *val = (int *) hash_tab_get(tab, key);
    g_assert(*val == arr[i]);
  }

  hash_tab_free(tab);

  return 0;
}