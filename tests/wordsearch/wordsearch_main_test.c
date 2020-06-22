#include "glib.h"
#include "wordsearch.h"
#include <locale.h>

typedef struct __sample {
    wchar_t *word;
    wchar_t *text;
    gssize pos;
} Sample;

static Sample begin_sample = {
    L"чемодан",
    L"чемоданkdfjvkjврпdcsbcdu",
    0
};

static Sample center_sample = {
    L"чемодан",
    L"kdчемоданfjvkварfnvlkdcsbcdu",
    2
};

static Sample end_sample = {
    L"чемодан",
    L"cduчемодан",
    3
};

static Sample short_sample = {
    L"чемодан",
    L"абv",
    -1
};

static Sample short2_sample = {
    L"чемодан",
    L"чем",
    -1
};

static Sample empty_sample = {
    L"",
    L"kkjn",
    -1
};

static Sample empty2_sample = {
    L"чемодан",
    L"",
    -1
};

static Sample empty3_sample = {
    L"",
    L"",
    0
};

static Sample eqlen_sample = {
    L"ggkjhg",
    L"iupoii",
    -1
};

static Sample ltlen_sample = {
    L"gg",
    L"gvhgvjh",
    -1
};

static Sample gtlen_sample = {
    L"hgjhgvjh",
    L"jhgvj",
    -1
};

static void begin_test(void)
{
  g_assert(wordsearch(begin_sample.word, begin_sample.text) == begin_sample.pos);
}

static void center_test(void)
{
  g_assert(wordsearch(center_sample.word, center_sample.text) == center_sample.pos);
}

static void end_test(void)
{
  g_assert(wordsearch(end_sample.word, end_sample.text) == end_sample.pos);
}

static void short_test(void)
{
  g_assert(wordsearch(short_sample.word, short_sample.text) == short_sample.pos);
}

static void short2_test(void)
{
  g_assert(wordsearch(short2_sample.word, short2_sample.text) == short2_sample.pos);
}

static void empty_test(void)
{
  g_assert(wordsearch(empty_sample.word, empty_sample.text) == empty_sample.pos);
}

static void empty2_test(void)
{
  g_assert(wordsearch(empty2_sample.word, empty2_sample.text) == empty2_sample.pos);
}

static void empty3_test(void)
{
  g_assert(wordsearch(empty3_sample.word, empty3_sample.text) == empty3_sample.pos);
}

static void eqlen_test(void)
{
  g_assert(wordsearch(eqlen_sample.word, eqlen_sample.text) == eqlen_sample.pos);
}

static void ltlen_test(void)
{
  g_assert(wordsearch(ltlen_sample.word, ltlen_sample.text) == ltlen_sample.pos);
}

static void gtlen_test(void)
{
  g_assert(wordsearch(gtlen_sample.word, gtlen_sample.text) == gtlen_sample.pos);
}

int main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");

  g_test_init(&argc, &argv, NULL);

  g_test_add_func("/wordsearch/begin", begin_test);
  g_test_add_func("/wordsearch/center", center_test);
  g_test_add_func("/wordsearch/end", end_test);
  g_test_add_func("/wordsearch/short", end_test);
  g_test_add_func("/wordsearch/short2", end_test);
  g_test_add_func("/wordsearch/empty", empty_test);
  g_test_add_func("/wordsearch/empty2", empty2_test);
  g_test_add_func("/wordsearch/empty3", empty3_test);
  g_test_add_func("/wordsearch/eqlen", eqlen_test);
  g_test_add_func("/wordsearch/ltlen", ltlen_test);
  g_test_add_func("/wordsearch/gtlen", gtlen_test);

  return g_test_run();
}
