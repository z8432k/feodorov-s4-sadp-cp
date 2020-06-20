#include "include/btree.h"

typedef gboolean (*BTreeInternalTraverseFunc) (
    BTreeNode *node,
    gpointer data);

typedef struct {
    BTreeNode *node;
    gconstpointer key;
} InternalLookupContext;

static inline void btree_try_free_key(BTreeNode *node)
{
  if (node->tree->key_destroy_func) {
    node->tree->key_destroy_func(node->key);
  }
}

static inline void btree_try_free_value(BTreeNode *node)
{
  if (node->tree->value_destroy_func) {
    node->tree->value_destroy_func(node->value);
  }
}

static gboolean btree_free_node(BTreeNode *node, gpointer user_data)
{
  btree_try_free_key(node);
  btree_try_free_value(node);

  g_free(node);

  g_print("BTreeNode destroyed.\n");

  return FALSE;
}

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
    btree_try_free_key(current_node);
    current_node->key = new_node->key;

    btree_try_free_value(current_node);
    current_node->value = new_node->value;

    g_free(new_node);
  }
}

static void btree_traverse_in_order(BTreeNode *current_node, GTraverseFunc func, gpointer user_data)
{
  if (current_node->left != NULL) {
    btree_traverse_in_order(current_node->left, func, user_data);
  }

  if (func(current_node->key, current_node->value, user_data)) {
    return;
  }

  if (current_node->right != NULL) {
    btree_traverse_in_order(current_node->right, func, user_data);
  }
}

static void btree_internal_traverse_in_order(BTreeNode *current_node, BTreeInternalTraverseFunc func, gpointer user_data)
{
  if (current_node->left != NULL) {
      btree_internal_traverse_in_order(current_node->left, func, user_data);
    }

  if (func(current_node, user_data)) {
      return;
    }

  if (current_node->right != NULL) {
      btree_internal_traverse_in_order(current_node->right, func, user_data);
  }
}

static gboolean btree_internal_lookup_node(BTreeNode *node, gpointer user_data)
{
  InternalLookupContext *context = user_data;

  gint comp_result = node->tree->comparator (context->key, node->key, NULL);

  if (!comp_result)
    {
      context->node = node;

      return TRUE;
    }

  return FALSE;
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

void btree_foreach(BTree *tree, GTraverseFunc func, gpointer user_data)
{
  btree_traverse_in_order(tree->root, func, user_data);
}

void btree_destroy(BTree *tree)
{
  if (tree->root) {
    btree_internal_traverse_in_order(tree->root, btree_free_node, NULL);
  }

  g_free(tree);
  g_print("BTree destroyed.\n");
}

gpointer btree_lookup(BTree *tree, gconstpointer key)
{
  InternalLookupContext context = {
    NULL,
    key
  };

  btree_internal_traverse_in_order(tree->root, btree_internal_lookup_node, &context);

  if (context.node) {
      return context.node->value;
  }

  return NULL;
}

static void btree_find_right_deep_node(BTreeNode *current, BTreeNode *target)
{
  if (current->right) {
    btree_find_right_deep_node(current->right, target);
  }

  if (current->left) {
    // replace current with left
    current->parent->right = current->left;
    current->left->parent = current->parent;
    current->left = NULL;
  }

  // replace target with current
  current->parent = target->parent;
  current->right = target->right;
  current->left = target->left;

  if (target->parent) {
    if (target->parent->right == target) {
      target->parent->right = current;
    }
    else {
      target->parent->left= current;
    }
  }
}

gboolean btree_remove(BTree *tree, gconstpointer key)
{
  InternalLookupContext context = {
      NULL,
      key
  };

  btree_internal_traverse_in_order(tree->root, btree_internal_lookup_node, &context);

  if (!context.node) {
      return FALSE;
  }

  BTreeNode *target = context.node;

  // Has no childs
  if (!(target->left || target->right)) {
    if (target->parent) {
      if (target->parent->right == target) {
        target->parent->right = NULL;
      }
      else {
        target->parent->left= NULL;
      }
    }
    btree_free_node(target, NULL); // free
  }
  // has one children
  else if (!(target->left && target->right)) {
    BTreeNode *child = target->left ? target->left : target->right;

    // set parent on single child
    child->parent = target->parent;

    if (target->parent) {
      if (target->parent->right == target) {
        target->parent->right = child;
      }
      else {
        target->parent->left = child;
      }
    }

    btree_free_node(target, NULL); // free
  }
  // has two childs
  else {
    // left child has no right descendant
    if (!target->left->right) {
      // set new left parent
      target->left->parent = target->parent;
      // set new left right
      target->left->right = target->right;
      // set new parent of right target descendant
      target->right->parent = target->left;

      if (target->parent) {
        if (target->parent->right == target) {
          target->parent->right = target->left;
        }
        else {
          target->parent->left= target->left;
        }
      }

      btree_free_node(target, NULL); // free
    }
    // left child has right descendant
    else {
      btree_find_right_deep_node(target->left, target);
    }
  }

  return TRUE;
}
