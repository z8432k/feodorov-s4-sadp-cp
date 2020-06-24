#include <glib.h>

typedef struct avltree {
    int key;
    char *value;
    int height;
    struct avltree *left;
    struct avltree *right;
} AVLTree;

struct avltree *avltree_add(struct avltree *tree, int key, char *value);
struct avltree *avltree_rightleft_rotate(struct avltree *tree);
struct avltree *avltree_leftright_rotate(struct avltree *tree);
struct avltree *avltree_left_rotate(struct avltree *tree);
struct avltree *avltree_right_rotate(struct avltree *tree);
int avltree_height(struct avltree *tree);
struct avltree *avltree_create(int key,char *value);
struct avltree *avltree_lookup(struct avltree *tree, int key);
void btree2_traverse_in_order(struct avltree *current_node, GTraverseFunc func, gpointer user_data);
void btree2_traverse_pre_order(struct avltree *current_node, GTraverseFunc func, gpointer user_data);
