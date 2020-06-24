#include <glib.h>

#define avltree_new(comparator) (avltree_new_full(comparator, NULL, NULL))

typedef struct __avl_tree AVLTree;
typedef struct __avltree_node AVLTreeNode;

struct __avl_tree {
    AVLTreeNode *root;
    GCompareDataFunc comparator;
    GDestroyNotify key_destroy_func;
    GDestroyNotify value_destroy_func;
};

struct __avltree_node {
    AVLTree *tree;
    gpointer key;
    gpointer value;
    gsize height;
    AVLTreeNode *left;
    AVLTreeNode *right;
};

AVLTree* avltree_new_full(GCompareDataFunc comparator, GDestroyNotify key_destroy_func, GDestroyNotify value_destroy_func);
void avltree_add(AVLTree *tree, gpointer key, gpointer value);
gpointer avltree_lookup(AVLTree *tree, gpointer key);
void avltree_foreach(AVLTree *tree, GTraverseFunc func, gpointer user_data);
void avltree_foreach_pre(AVLTree *tree, GTraverseFunc func, gpointer user_data);
