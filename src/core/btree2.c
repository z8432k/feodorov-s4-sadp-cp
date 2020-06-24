#include <stdlib.h>
#include "btree2.h"

static int imax2(int a, int b)
{
  if (a > b) {
      return a;
    }
  else {
      return b;
    }
}

AVLTree *avltree_lookup(AVLTree *tree, int key)
{
  while (tree != NULL) {
      if (key == tree->key) {
          return tree;
        } else if (key < tree->key) {
          tree = tree->left;
        } else {
          tree = tree->right;
        }
    }
  return tree;
}

AVLTree *avltree_create(int key,char *value)
{
  AVLTree *node;
  node = malloc(sizeof(*node));
  if (node != NULL) {
      node->key = key;
      node->value = value;
      node->left = NULL;
      node->right = NULL;
      node->height = 0;
    }
  return node;
}


int avltree_height(AVLTree *tree)
{
  return (tree != NULL) ? tree->height : -1;
}
int avltree_balance(AVLTree *tree)
{
  return avltree_height(tree->left) - avltree_height(tree->right);
}

AVLTree *avltree_right_rotate(AVLTree *tree)
{
  AVLTree *left;
  left = tree->left;
  tree->left = left->right;
  left->right = tree;
  tree->height = imax2(
      avltree_height(tree->left),
      avltree_height(tree->right)) + 1;
  left->height = imax2(
      avltree_height(left->left),
      tree->height) + 1;
  return left;
}

AVLTree *avltree_left_rotate(AVLTree *tree)
{
  AVLTree *right;
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

AVLTree *avltree_leftright_rotate(AVLTree *tree)
{
  tree->left = avltree_left_rotate(tree->left);
  return avltree_right_rotate(tree);
}

AVLTree *avltree_rightleft_rotate(AVLTree *tree)
{
  tree->right = avltree_right_rotate(tree->right);
  return avltree_left_rotate(tree);
}

AVLTree *avltree_add(AVLTree *tree, int key, char *value)
{
  if (tree == NULL) {
// Insert new item
      return avltree_create(key, value);
    }

  if (key < tree->key) {
// Insert into left subtree
      tree->left = avltree_add(tree->left, key, value);

      if (avltree_height(tree->left) - avltree_height(tree->right) == 2) {
// Subtree is unbalanced
          if (key < tree->left->key) {
// Left left case
              tree = avltree_right_rotate(tree);
            } else {
// Left right case
              tree = avltree_leftright_rotate(tree);
            }
        }
    }
  else if (key > tree->key) {
// Insert into right subtree
      tree->right = avltree_add(tree->right, key, value);

      if (avltree_height(tree->right) - avltree_height(tree->left) == 2) {
// Subtree is unbalanced
          if (key > tree->right->key) {
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







AVLTree *avltree_remove(AVLTree *tree, int key, char *value) {
  if (tree == NULL) {
      // Insert new item
      return avltree_create(key, value);
    }

  if (key < tree->key) {
// Insert into left subtree
      tree->left = avltree_add(tree->left, key, value);
      if (avltree_height(tree->left) - avltree_height(tree->right) == 2) {
// Subtree is unbalanced
          if (key < tree->left->key) {
// Left left case
              tree = avltree_right_rotate(tree);
            } else {
// Left right case
              tree = avltree_leftright_rotate(tree);
            }
        }
    }


  else if (key > tree->key) {
// Insert into right subtree
      tree->right = avltree_add(tree->right, key, value);
      if (avltree_height(tree->right) - avltree_height(tree->left) == 2) {
// Subtree is unbalanced
          if (key > tree->right->key) {
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



void btree2_traverse_in_order(AVLTree *current_node, GTraverseFunc func, gpointer user_data)
{
  if (current_node->left != NULL) {
      btree2_traverse_in_order(current_node->left, func, user_data);
    }

  if (func(current_node->key, current_node->value, user_data)) {
      return;
    }

  if (current_node->right != NULL) {
      btree2_traverse_in_order(current_node->right, func, user_data);
    }
}

void btree2_traverse_pre_order(AVLTree *current_node, GTraverseFunc func, gpointer user_data)
{
  if (current_node->left != NULL) {
      btree2_traverse_pre_order(current_node->left, func, user_data);
    }

  if (current_node->right != NULL) {
      btree2_traverse_pre_order(current_node->right, func, user_data);
    }

  if (func(current_node->key, current_node->value, user_data)) {
      return;
    }
}
