#include "include/hashtab.h"

// TODO: Automatic re-hash
#define HASH_TAB_INIT_SIZE 256

#define hash_tab_get_cell(tab, idx) ((HashTabCell_t *) &tab->data[idx])
#define hash_tab_compare_key(cell, k) (g_string_equal(cell->key, k))
#define hash_tab_mark_deleted(cell) (cell->deleted = TRUE)
#define set_cell_data(cell, key, data)  \
{                                       \
  cell->key = key;                      \
  cell->data = data;                    \
  cell->deleted = FALSE;                \
}

static inline gboolean is_vacant_cell(HashTabCell_t *cell)
{
  return cell->deleted || cell->key == NULL;
}

static inline gsize hash1(HashTab_t *tab, GString *key)
{
  gsize h = 0;
  guchar *p;

  for (p = (guchar *) key->str; *p != '\0'; p++) {
    h = h + *p;
  }

  return h % tab->size;
}

static inline gsize hash2(HashTab_t *tab, GString *key)
{
  gsize h = 0;
  guchar *p;

  for (p = (guchar *) key->str; *p != '\0'; p++) {
    h = h + *p * *p;
  }

  if (!(h % 2)) {
    h++;
  }

  return h % tab->size;
}

static inline gsize hash(HashTab_t *tab, gsize step, GString *key)
{
  gsize idx = (hash1(tab, key) + step * hash2(tab, key)) % tab->size;

  return idx;
}

static gssize hash_tab_get_internal(HashTab_t *tab, GString *key)
{
  HashTabCell_t *currentCell;
  gsize idx;
  for (gsize i = 0; i < tab->size; i++) {
    idx = hash(tab, i, key);

    currentCell = hash_tab_get_cell(tab, idx);

    if (!is_vacant_cell(currentCell)) {
      if (hash_tab_compare_key(currentCell, key)) {
        return idx;
      }
      else {
        continue;
      }
    }
    else {
      return -1;
    }
  }

  return -1;
}

HashTab_t* hash_tab_new()
{
  HashTab_t *tab = g_new(HashTab_t, 1);

  tab->size = HASH_TAB_INIT_SIZE;
  tab->data = g_new0(HashTabCell_t, HASH_TAB_INIT_SIZE);

  return tab;
}

void hash_tab_free(HashTab_t *tab)
{
  g_free(tab->data);
  g_free(tab);
}

gssize hash_tab_set(HashTab_t *tab, GString *key, gconstpointer data)
{
  HashTabCell_t *currentCell;
  gsize idx;
  for (gsize i = 0; i < tab->size; i++) {
    idx = hash(tab, i, key);

    currentCell = hash_tab_get_cell(tab, idx);

    if (is_vacant_cell(currentCell)) {
      set_cell_data(currentCell, key, data);

      return idx;
    }
    else {
      continue;
    }
  }

  return -1; // error. Table is full.
}

gssize hash_tab_remove(HashTab_t *tab, GString *key)
{
  gssize idx = hash_tab_get_internal(tab, key);

  if (idx > -1) {
    HashTabCell_t *cell = hash_tab_get_cell(tab, idx);
    hash_tab_mark_deleted(cell);
  }

  return idx;
}

gconstpointer hash_tab_get(HashTab_t *tab, GString *key)
{
  gssize idx = hash_tab_get_internal(tab, key);

  if (idx < 0) {
    return NULL;
  }

  HashTabCell_t *currentCell = hash_tab_get_cell(tab, idx);

  return currentCell->data;
}