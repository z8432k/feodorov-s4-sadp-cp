#include <glib.h>

#include "../include/hashtab.h"

int main(void)
{
  HashTab_t *tab = hash_tab_new();

  hash_tab_free(tab);
}