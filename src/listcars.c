#include <stdio.h>
#include <locale.h>

#include "stringify_json.h"
#include "storage.h"

#define GETTEXT_PACKAGE "gtk20"

int main(void)
{
  setlocale(LC_ALL, "");

  printf("%s\n", cars_stringify_json(data_load()));
}
