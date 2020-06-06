#include <stdio.h>
#include <locale.h>
#include <glib.h>

static void process()
{
  printf("Выполнено успешно.");
}

int main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");

  process();
}

