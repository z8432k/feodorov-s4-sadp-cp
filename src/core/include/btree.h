//#ifndef _BTREE_H_
//#define _BTREE_H_

#include <glib.h>

#define btree_new(comparator) (btree_new_full(comparator, NULL, NULL))

typedef struct __BTree BTree;
typedef struct __BTreeNode BTreeNode;

struct __BTree {
    BTreeNode *root;
    GCompareDataFunc comparator;
    GDestroyNotify key_destroy_func;
    GDestroyNotify value_destroy_func;
};

BTree* btree_new_full(GCompareDataFunc comparator, GDestroyNotify key_destroy_func, GDestroyNotify value_destroy_func);
void btree_destroy(BTree *btree);
void btree_insert(BTree *tree, gpointer key, gpointer value);
gboolean btree_remove(BTree *tree, gconstpointer key);
gpointer btree_lookup(BTree *tree, gconstpointer key);
void btree_foreach(BTree *tree, GTraverseFunc func, gpointer user_data);
void btree_foreach_pre(BTree *tree, GTraverseFunc func, gpointer user_data);


//#endif //_BTREE_H_
