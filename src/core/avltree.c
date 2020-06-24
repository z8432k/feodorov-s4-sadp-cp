#include "avltree.h"

#define avltree_node_new() (g_new0(AVLTreeNode, 1))

typedef gboolean (*BTreeInternalTraverseFunc) (
    AVLTreeNode *node,
    gpointer data);

static int imax2(int a, int b)
{
  if (a > b) {
      return a;
    }
  else {
      return b;
    }
}

static inline void avltree_try_free_key(AVLTreeNode *node)
{
  if (node->tree->key_destroy_func) {
    node->tree->key_destroy_func(node->key);
  }
}

static inline void avltree_try_free_value(AVLTreeNode *node)
{
  if (node->tree->value_destroy_func) {
    node->tree->value_destroy_func(node->value);
  }
}

static gboolean avltree_free_node(AVLTreeNode *node, gpointer user_data)
{
  avltree_try_free_key(node);
  avltree_try_free_value(node);

  g_free(node);

  return FALSE;
}

static void avltree_traverse_in_order(AVLTreeNode *current_node, GTraverseFunc func, gpointer user_data)
{
  if (current_node->left != NULL) {
      avltree_traverse_in_order(current_node->left, func, user_data);
    }

  if (func(current_node->key, current_node->value, user_data)) {
      return;
    }

  if (current_node->right != NULL) {
      avltree_traverse_in_order(current_node->right, func, user_data);
    }
}

static void avltree_traverse_pre_order(AVLTreeNode *current_node, GTraverseFunc func, gpointer user_data)
{
  if (current_node->left != NULL) {
      avltree_traverse_pre_order(current_node->left, func, user_data);
    }

  if (current_node->right != NULL) {
      avltree_traverse_pre_order(current_node->right, func, user_data);
    }

  if (func(current_node->key, current_node->value, user_data)) {
      return;
    }
}

AVLTree* avltree_new_full(GCompareDataFunc comparator, GDestroyNotify key_destroy_func, GDestroyNotify value_destroy_func)
{
  AVLTree *tree = g_new0(AVLTree, 1);

  tree->comparator = comparator;
  tree->key_destroy_func = key_destroy_func;
  tree->value_destroy_func = value_destroy_func;

  return tree;
}

void avltree_foreach(AVLTree *tree, GTraverseFunc func, gpointer user_data)
{
  avltree_traverse_in_order(tree->root, func, user_data);
}

void avltree_foreach_pre(AVLTree *tree, GTraverseFunc func, gpointer user_data)
{
  avltree_traverse_pre_order(tree->root, func, user_data);
}


void avltree_destroy(AVLTree *tree)
{
  if (tree->root) {
    avltree_traverse_in_order(tree->root, avltree_free_node, NULL);
  }

  g_free(tree);
}


static int avltree_height(AVLTreeNode *node)
{
  return (node != NULL) ? node->height : -1;
}

static int avltree_balance(AVLTreeNode *tree)
{
  return avltree_height(tree->left) - avltree_height(tree->right);
}

static AVLTreeNode *avltree_right_rotate(AVLTreeNode *node)
{
  AVLTreeNode *left;
  left = node->left;
  node->left = left->right;
  left->right = node;
  node->height = imax2(
      avltree_height(node->left),
      avltree_height(node->right)) + 1;
  left->height = imax2(
      avltree_height(left->left),
      node->height) + 1;
  return left;
}

static AVLTreeNode *avltree_left_rotate(AVLTreeNode *tree)
{
  AVLTreeNode *right;
  right = tree->right;
  tree->right = right->left;
  right->left = tree;
  tree->height = imax2(
      avltree_height(tree->left),
      avltree_height(tree->right)) + 1;
  right->height = imax2(
      avltree_height(right->right),
      tree->height) + 1;
  return right;
}

static AVLTreeNode *avltree_leftright_rotate(AVLTreeNode *tree)
{
  tree->left = avltree_left_rotate(tree->left);
  return avltree_right_rotate(tree);
}

static AVLTreeNode *avltree_rightleft_rotate(AVLTreeNode *tree)
{
  tree->right = avltree_right_rotate(tree->right);
  return avltree_left_rotate(tree);
}

static AVLTreeNode *avltree_add_deep(AVLTreeNode *tree, AVLTreeNode *new_node)
{
  if(!tree) {
    return new_node;
  }

  gint comp_result = tree->tree->comparator(new_node->key, tree->key, NULL);

  if (comp_result < 0) {
// Insert into left subtree
    tree->left = avltree_add_deep(tree->left, new_node);

    if (avltree_height(tree->left) - avltree_height(tree->right) == 2) {
// Subtree is unbalanced
        if (tree->tree->comparator(new_node->key, tree->left->key, NULL) < 0) {
// Left left case
            tree = avltree_right_rotate(tree);
          } else {
// Left right case
            tree = avltree_leftright_rotate(tree);
          }
      }
  }
  else if (comp_result > 0) {
// Insert into right subtree
    tree->right = avltree_add_deep(tree->right, new_node);

    if (avltree_height(tree->right) - avltree_height(tree->left) == 2) {
// Subtree is unbalanced
        if (tree->tree->comparator(new_node->key, tree->right->key, NULL) > 0) {
// Right right case
            tree = avltree_left_rotate(tree);
          } else {
// Right left case
            tree = avltree_rightleft_rotate(tree);
          }
      }
  }

  tree->height = imax2(
      avltree_height(tree->left),
      avltree_height(tree->right)
  ) + 1;

  return tree;
}

void avltree_add(AVLTree *tree, gpointer key, gpointer value)
{

  AVLTreeNode *new_node = avltree_node_new();

  new_node->tree = tree;
  new_node->key = key;
  new_node->value = value;

  tree->root = avltree_add_deep(tree->root, new_node);
}

