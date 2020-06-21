#include <stdio.h>
#include <locale.h>

#include "common/include/stringify_json.h"
#include "storage/include/json_store.h"

#define GETTEXT_PACKAGE "gtk20"

int main(void)
{
  setlocale(LC_ALL, "");

  printf("%s\n", clients_stringify_json(load_data()));
}
