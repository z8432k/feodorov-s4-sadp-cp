#include <stdio.h>
#include <locale.h>

#include "include/store.h"

#define GETTEXT_PACKAGE "gtk20"

int main(void)
{
  setlocale(LC_ALL, "");

  printf("%s\n", clients_stringify(load_data()));
}
