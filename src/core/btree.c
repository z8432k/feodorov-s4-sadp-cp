#include "btree.h"

#define btree_node_new() (g_new0(BTreeNode, 1))

struct __BTreeNode {
    BTree *tree;
    gpointer key;
    gpointer value;
    BTreeNode *left;
    BTreeNode *right;
    gsize height;
};

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

  // g_print("BTreeNode destroyed.\n");

  return FALSE;
}

static gsize btree_height(BTreeNode *node)
{
  return node ? node->height : 0;
}

static gshort btree_bfactor(BTreeNode *node)
{
  return btree_height(node->right)-btree_height(node->left);
}

static void btree_fixheight(BTreeNode *node)
{
  gsize hl = btree_height(node->left);
  gsize hr = btree_height(node->right);

  node->height = (hl > hr ? hl : hr) + 1;
}

static BTreeNode* btree_findmin(BTreeNode *node)
{
  return node->left ? btree_findmin(node->left) : node;
}

static BTreeNode* btree_rotate_left(BTreeNode *node)
{
  BTreeNode * tmp = node->right;

  node->right = tmp->left;
  tmp->left = node;

  btree_fixheight(node);
  btree_fixheight(tmp);

  return tmp;
}

static BTreeNode* btree_rotate_right(BTreeNode *node)
{
  BTreeNode *tmp = node->left;

  node->left = tmp->right;
  tmp->right = node;

  btree_fixheight(node);
  btree_fixheight(tmp);

  return tmp;
}

static BTreeNode* btree_balance(BTreeNode *node)
{
  btree_fixheight(node);

  if (btree_bfactor(node) == 2) {
      if (btree_bfactor(node->right) < 0) {
          node->right = btree_rotate_right(node->right);
        }

      return btree_rotate_left(node);
    }

  if (btree_bfactor(node)==-2) {
      if (btree_bfactor(node->left) > 0) {
          node->left = btree_rotate_left(node->left);
        }

      return btree_rotate_right(node);
    }

  return node; // балансировка не нужна
}

static BTreeNode* btree_removemin(BTreeNode* node) // удаление узла с минимальным ключом из дерева p
{
  if( node->left==0 )
    return node->right;
  node->left = btree_removemin(node->left);
  return btree_balance(node);
}

static BTreeNode* btree_insert_deep(BTreeNode *node, BTreeNode *new_node)
{
  if(!node) {
    return new_node;
  }

  gint comp_result = node->tree->comparator(new_node->key, node->key, NULL);

  if(comp_result < 0) {
    node->left = btree_insert_deep(node->left, new_node);
  }
  else {
    node->right = btree_insert_deep(node->right, new_node);
  }

  return btree_balance(node);
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

static void btree_traverse_pre_order(BTreeNode *current_node, GTraverseFunc func, gpointer user_data)
{
  if (current_node->left != NULL) {
    btree_traverse_pre_order(current_node->left, func, user_data);
  }

  if (current_node->right != NULL) {
    btree_traverse_pre_order(current_node->right, func, user_data);
  }

  if (func(current_node->key, current_node->value, user_data)) {
    return;
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

  if (!comp_result) {
    context->node = node;

    return TRUE;
  }

  return FALSE;
}

static BTreeNode* _btree_remove_deep(BTreeNode *current_node, gconstpointer key)
{
  gint comp_result = current_node->tree->comparator(key, current_node->key, NULL);

  if (comp_result < 0) {
    //current_node->left = btree_remove_deep(current_node->left, key);
  }
  else if (comp_result > 0) {
    //current_node->right = btree_remove_deep(current_node->right, key);
  }
  else { // bingo!
    BTreeNode *left = current_node->left;
    BTreeNode *right = current_node->right;

    btree_free_node(current_node, NULL); // free

    if(!right) {
      return left;
    }

    BTreeNode *min = btree_findmin(right);
    min->right = btree_removemin(right);
    min->left = left;

    return btree_balance(min);
  }

  return btree_balance(current_node);
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

  tree->root = btree_insert_deep(tree->root, new_node);
}

void btree_foreach(BTree *tree, GTraverseFunc func, gpointer user_data)
{
  btree_traverse_in_order(tree->root, func, user_data);
}

void btree_foreach_pre(BTree *tree, GTraverseFunc func, gpointer user_data)
{
  btree_traverse_pre_order(tree->root, func, user_data);
}

void btree_destroy(BTree *tree)
{
  if (tree->root) {
    btree_internal_traverse_in_order(tree->root, btree_free_node, NULL);
  }

  g_free(tree);
  // g_print("BTree destroyed.\n");
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

static BTreeNode* btree_remove_deep_(BTreeNode *node, gconstpointer key)
{
  if (!node)
    {
      return 0;
    }

  gint comp_result = node->tree->comparator (key, node->key, NULL);

  if (comp_result < 0)
    {
      node->left = btree_remove_deep_ (node->left, key);
    }
  else if (comp_result > 0)
    {
      node->right = btree_remove_deep_ (node->right, key);
    }
  else
    {
      BTreeNode *q = node->left;
      BTreeNode *r = node->right;

      btree_free_node (node, NULL);

      if (!r)
        {
          return q;
        }

      BTreeNode *min = btree_findmin (r);
      min->right = btree_removemin (r);
      min->left = q;

      return btree_balance (min);
    }

  return btree_balance(node);
}

gboolean btree_remove(BTree *tree, gconstpointer key)
{
  btree_remove_deep_(tree->root, key);

  return 0;
}
