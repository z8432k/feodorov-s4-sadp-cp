#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <glib.h>

#include "include/store.h"

#define GETTEXT_PACKAGE "gtk20"
#include <glib/gi18n-lib.h>

int main(void)
{
  setlocale(LC_ALL, "");

  printf("%s\n", clients_stringify(load_data()));
}
