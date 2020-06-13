#include "include/hashtab.h"

#define HASH_TAB_INIT_SIZE 16
#define HASH_TAB_ROW_SIZE 8
#define HASH_TAB_KEY_SIZE 6
#define KEY_FLAG_POS 7

typedef struct {
    GString *key;
    gconstpointer data;
    guchar deleted : 1;
} HashTabCell_t;

static inline gboolean is_vacant_cell(HashTabCell_t *cell)
{
  return cell->deleted || cell->key == NULL;
}

static inline void mark_deleted_cell(HashTabCell_t *cell)
{
  cell->deleted = TRUE;
}

static inline void set_cell_data(HashTabCell_t *cell, GString *key, gconstpointer data)
{
  cell->key = key;
  cell->data = data;
  cell->deleted = FALSE;
}

static gsize hash1(HashTab_t *tab, GString *key)
{
  gsize h = 0;
  guchar *p;

  for (p = (guchar *) key; *p != '\0'; p++) {
    h = h + *p;
  }

  return h % tab->size;
}

static gsize hash2(HashTab_t *tab, GString *key)
{
  gsize h = 0;
  guchar *p;

  for (p = (guchar *) key; *p != '\0'; p++) {
    h = h + *p * *p;
  }

  if (!(h % 2)) {
    h++;
  }

  return h % tab->size;
}

static gsize hash(HashTab_t *tab, gsize step, GString *key)
{
  gsize idx = (hash1(tab, key) + step * hash2(tab, key)) % tab->size;

  return idx;
}

static inline gboolean compare_key(HashTabCell_t *cell, GString *key)
{
  return g_string_equal(cell->key, key);
}

static gssize hash_tab_get_internal(HashTab_t *tab, GString *key)
{
  HashTabCell_t currentCell;
  gsize idx;
  for (gsize i = 0; i < tab->size; i++) {
    idx = hash(tab, i, key);

    currentCell = g_array_index(tab->data, HashTabCell_t, idx);

    if (!is_vacant_cell(&currentCell)) {
      if (compare_key(&currentCell, key)) {
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
  tab->data = g_array_sized_new(FALSE, TRUE, sizeof(HashTabCell_t), HASH_TAB_INIT_SIZE);

  return tab;
}

void hash_tab_free(HashTab_t *tab)
{
  g_array_free(tab->data, FALSE);
  g_free(tab);
}

gssize hash_tab_set(HashTab_t *tab, GString *key, gconstpointer data)
{
  HashTabCell_t currentCell;
  gsize idx;
  for (gsize i = 0; i < tab->size; i++) {
    idx = hash(tab, i, key);

    currentCell = g_array_index(tab->data, HashTabCell_t, idx);

    if (is_vacant_cell(&currentCell)) {
      set_cell_data(&currentCell, key, data);

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
    HashTabCell_t cell = g_array_index(tab->data, HashTabCell_t, idx);

    mark_deleted_cell(&cell);
  }

  return idx;
}

gconstpointer hash_tab_get(HashTab_t *tab, GString *key)
{
  gsize idx = hash_tab_get_internal(tab, key);

  if (idx > 0) {
    HashTabCell_t currentCell = g_array_index(tab->data, HashTabCell_t, idx);

    return currentCell.data;
  }

  return NULL;
}