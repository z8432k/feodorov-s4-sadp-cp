#include <stdio.h>
#include <locale.h>

#include "stringify_json.h"
#include "rentcar.h"

#define GETTEXT_PACKAGE "gtk20"

int main(void)
{
  setlocale(LC_ALL, "");

  rentcar_init();

  printf("%s\n", cars_stringify_json(data_load()));
}
