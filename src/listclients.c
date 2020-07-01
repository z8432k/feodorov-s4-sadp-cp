#include <stdio.h>
#include <locale.h>

#include "rentcar.h"
#include "stringify_json.h"

#define GETTEXT_PACKAGE "gtk20"

int main(void)
{
  setlocale(LC_ALL, "");

  rentcar_init();

  printf("%s\n", clients_stringify_json(data_load()));
}
