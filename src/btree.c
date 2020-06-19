#include "include/btree.h"

static void btree_insert_deep(BTreeNode *current_node, BTreeNode *new_node)
{
  gint comp_result = current_node->tree->comparator(new_node->key, current_node->key, NULL);

  if (comp_result < 0) {
    if (current_node->left == NULL) {
      new_node->parent = current_node;
      current_node->left = new_node;
    }
    else {
      btree_insert_deep(current_node->left, new_node);
    }
  }
  else if (comp_result > 0) {
    if (current_node->right == NULL) {
      new_node->parent = current_node;
      current_node->right = new_node;
    }
    else {
      btree_insert_deep(current_node->right, new_node);
    }
  }
  else {
    if (current_node->tree->key_destroy_func) {
      current_node->tree->key_destroy_func(current_node->key);
    }
    current_node->key = new_node->key;

    if (current_node->tree->value_destroy_func) {
      current_node->tree->value_destroy_func(current_node->value);
    }
    current_node->value = new_node->value;

    g_free(new_node);
  }
}

BTree* btree_new_full(GCompareDataFunc comparator, GDestroyNotify key_destroy_func, GDestroyNotify value_destroy_func)
{
  BTree *tree = g_new0(BTree, 1);

  tree->comparator = comparator;
  tree->key_destroy_func = key_destroy_func;
  tree->value_destroy_func = value_destroy_func;

  return tree;
}

void btree_insert(BTree *tree, gpointer key, gpointer value)
{
  BTreeNode *new_node = btree_node_new();

  new_node->tree = tree;
  new_node->key = key;
  new_node->value = value;

  if (tree->root == NULL) {
    tree->root = new_node;
  }
  else {
    btree_insert_deep(tree->root, new_node);
  }
}
