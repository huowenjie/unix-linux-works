#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "binary-tree.h"

#define KEY_SIZE   64
#define VALUE_SIZE 32

// 遍历树的方式
#define PERORDER  1
#define INORDER   2
#define POSTORDER 3

// 一个不平衡的二叉树
struct value_st
{
	// 测试数据
	char value[VALUE_SIZE];
};

struct node_st
{
	char key[KEY_SIZE];	// 键

	TREE_NODE *left;	// 左子树
	TREE_NODE *right;	// 右子树
	int	left_count;		// 左子树儿子数量
	int right_count;	// 右子树儿子数量

	TREE_VALUE value;	// 节点数据
};

struct tree_st
{
	int count;			// 树子节点总数，包含根节点
	TREE_NODE *root;	// 根节点地址

	// 互斥锁
	pthread_mutex_t lock;
};

// 初始化节点对象
static void init_node(TREE_NODE *node, const char *key, const char *value)
{
	if (!node) {
		return;
	}

	if (!key || !key[0]) {
		return;
	}

	if (!value || !value[0]) {
		return;
	}

	memset(node->key, 0, KEY_SIZE);
	strncpy(node->key, key, KEY_SIZE);
	node->key[KEY_SIZE - 1] = '\0';

	node->left  = NULL;
	node->right = NULL;
	node->left_count  = 0;
	node->right_count = 0;

	memset(node->value.value, 0, VALUE_SIZE);
	strncpy(node->value.value, value, VALUE_SIZE);
	node->value.value[VALUE_SIZE - 1] = '\0';
}

static int add_node(TREE_NODE *node, const char *key, const char *value)
{
	TREE_NODE *root = NULL;
	int ret = 0;
	int comp_ret = 0;

	if (!node) {
		return -1;
	}

	if (!key || !key[0]) {
		return -1;
	}

	if (!value || !value[0]) {
		return -1;
	}

	root = node;

	if ((comp_ret = strncmp(key, root->key, KEY_SIZE)) < 0) {
		if (!root->left) {
			root->left = (TREE_NODE *)malloc(sizeof(TREE_NODE));
			if (!root->left) {
				return -1;
			}

			init_node(root->left, key, value);
			root->left_count += 1;
			return 0;
		}

		// 查找左子树
		if ((ret = add_node(root->left, key, value)) == 0) {
			root->left_count += 1;
		}
	} else if (comp_ret > 0) {
		if (!root->right) {
			// 创建右子树节点
			root->right = (TREE_NODE *)malloc(sizeof(TREE_NODE));
			if (!root->right) {
				return -1;
			}

			init_node(root->right, key, value);
			root->right_count += 1;
			return 0;
		}

		// 查找右子树
		if ((ret = add_node(root->right, key, value)) == 0) {
			root->right_count += 1;
		}
	} else {
		// 发现当前节点已经存在，则返回错误
		return -1;
	}

	return ret;
}

static int get_bottom_child_node(TREE_NODE *node, TREE_NODE **child, int comp)
{
	TREE_NODE *tmp = NULL;

	if (!node || !child) {
		return -1;
	}

	tmp = node;
	
	// 向左搜索
	if (comp < 0) {
		// 搜索至底部
		while (tmp->left) {
			tmp = tmp->left;
		}
	} else {
		// 向右搜索
		while (tmp->right) {
			tmp = tmp->right;
		}
	}

	*child = tmp;

	return 0;
}

static int del_node(TREE_NODE *node, TREE_NODE *parent, int comp, const char *key)
{
	TREE_NODE *root  = NULL;
	TREE_NODE *child = NULL;

	int ret = 0;
	int comp_ret = 0;

	if (!node) {
		return -1;
	}

	if (!key || !key[0]) {
		return -1;
	}

	root = node;

	if ((comp_ret = strncmp(key, root->key, KEY_SIZE)) < 0) {
		if (!root->left) {
			return -1;
		}

		// 查找左子树
		if ((ret = del_node(root->left, root, -1, key)) == 0) {
			root->left_count -= 1;
		}
	} else if (comp_ret > 0) {
		if (!root->right) {
			return -1;
		}

		// 查找右子树
		if ((ret = del_node(root->right, root, 1, key)) == 0) {
			root->right_count -= 1;
		}
	} else {
		if (parent && comp < 0) {
			if (!root->right) {
				parent->left = root->left;
				free(root);
				return 0;
			}

			parent->left = root->right;
			if (get_bottom_child_node(root->right, &child, -1) < 0) {
				return -1;
			}

			if (child) {
				child->left = root->left;
			}

			free(root);
			return 0;
		} else if (parent && comp > 0) {
			if (!root->left) {
				parent->right = root->right;
				free(root);
				return 0;
			}

			parent->right = root->left;
			if (get_bottom_child_node(root->left, &child, 1) < 0) {
				return -1;
			}

			if (child) {
				child->right = root->right;
			}

			free(root);
			return 0;
		} else {
			return -1;
		}
	}

	return ret;
}

static void lookup_node(TREE_NODE *node, const char *key, TREE_NODE **target)
{
	int comp_ret = 0;
	TREE_NODE *root = NULL;

	if (!node || !target) {
		return;
	}

	if (!key || !key[0]) {
		return;
	}

	root = node;

	if ((comp_ret = strncmp(key, root->key, KEY_SIZE)) < 0) {
		if (root->left) {
			lookup_node(root->left, key, target);
		}
	} else if (comp_ret > 0) {
		if (root->right) {
			lookup_node(root->right, key, target);
		}
	} else {
		*target = root;
	}
}

static void handler(TREE_NODE *node)
{
	if (!node) {
		return;
	}

	printf("node key = %s, value = %s\n", node->key, node->value.value);
	free(node);
}

// type = 1 前序遍历
// type = 2 中序遍历
// type = 3 后序遍历
static void traverse_tree(int type, TREE_NODE *root, void (*handle)(TREE_NODE *))
{
	if (!root) {
		return;
	}

	if (type == PERORDER && handle) {
		handle(root);
	}

	traverse_tree(type, root->left, handle);

	if (type == INORDER && handle) {
		handle(root);
	}

	traverse_tree(type, root->right, handle);

	if (type == POSTORDER && handle) {
		handle(root);
	}
}

// ============================================================================

BINARY_TREE * initialize()
{
	// 创建根节点，初始化互斥锁
	BINARY_TREE *tree = (BINARY_TREE *)malloc(sizeof(BINARY_TREE));

	if (!tree) {
		return NULL;
	}

	// 初始化子树成员
	tree->count = 0;

	// 初始化互斥量
	pthread_mutex_init(&(tree->lock), NULL);

	return tree;
}

int terminate(BINARY_TREE *tree)
{
	int i = 0;

	if (!tree) {
		return -1;
	}

	// 释放子树节点
	if (tree->count > 0 && tree->root) {
		traverse_tree(POSTORDER, tree->root, handler);
	}

	pthread_mutex_destroy(&(tree->lock));

	// 释放树
	free(tree);

	return 0;
}

int add(BINARY_TREE *tree, const char *key,  const char *value)
{
	TREE_NODE *root = NULL;
	int ret = 0;

	if (!tree) {
		return -1;
	}

	if (!key || !key[0]) {
		return -1;
	}

	if (!value || !value[0]) {
		return -1;
	}

	pthread_mutex_lock(&(tree->lock));

	// 获取根节点
	root = tree->root;
	
	do {
		if (!root) {
			root = (TREE_NODE *)malloc(sizeof(TREE_NODE));
			if (!root) {
				// 内存分配错误
				ret = -1;
				break;
			}

			// 初始化根节点数据
			init_node(root, key, value);
			tree->root = root;
			break;
		}

		ret = add_node(root, key, value);
	}
	while (0);

	if (!ret) {
		tree->count += 1;
	}

	pthread_mutex_unlock(&(tree->lock));

	return ret;
}

int del(BINARY_TREE *tree,  const char *key)
{
	int ret = 0;
	int comp_ret = 0;

	TREE_NODE *root  = NULL;
	TREE_NODE *tmp   = NULL;
	TREE_NODE *child = NULL;

	if (!tree || !key || !key[0]) {
		return -1;
	}

	pthread_mutex_lock(&(tree->lock));

	root = tree->root;

	do {
		if ((comp_ret = strncmp(key, root->key, KEY_SIZE)) < 0) {
		    if (!root->left) {
				ret = -1;
			    break;
		    }

		    // 查找左子树
		    if ((ret = del_node(root->left, root, -1, key)) == 0) {
			    root->left_count -= 1;
		    }
        } else if (comp_ret > 0) {
		    if (!root->right) {
			    ret = -1;
		    	break;
			}

    		// 查找右子树
		    if ((ret = del_node(root->right, root, 1, key)) == 0) {
			    root->right_count -= 1;
		    }
	    } else {
			if (!root->right) {
				tree->root = root->left;
				free(root);
				break;
			}

		    // 处理根节点移除情况
	    	tree->root = root->right;
			
			// 搜索右节点的左节点
			if (get_bottom_child_node(root->right, &child, -1) < 0) {
				ret = -1;
				break;
			}

			if (child) {
				child->left = root->left;
			}

			free(root);
	    }
	}
	while (0);
	
    if (!ret) {
		tree->count -= 1;
	}

	pthread_mutex_unlock(&(tree->lock));

	return ret;
}

int lookup(BINARY_TREE *tree, const char *key, char *value, size_t buff_len)
{
	TREE_NODE *root = NULL;
	TREE_NODE *target = NULL;
	size_t size = 0;

	if (!tree) {
		return -1;
	}

	if (!key || !key[0]) {
		return -1;
	}

	root = tree->root;
	lookup_node(root, key, &target);

	if (!value) {
		return 0;
	}

	if (target) {
		size = (buff_len > VALUE_SIZE) ? VALUE_SIZE : buff_len;
		strncpy(value, target->value.value, size);
	}

	return 0;
}

int get_key(TREE_NODE *node, char *key, size_t buff_len) 
{
	size_t size = 0;

	if (!node || !key) {
		return -1;
	}

	size = (buff_len > KEY_SIZE) ? KEY_SIZE : buff_len;
	strncpy(key, node->key, size);

	return 0;
}

int get_value(TREE_NODE *node, char *value, size_t buff_len) 
{
	size_t size = 0;

	if (!node || !value) {
		return -1;
	}

	size = (buff_len > VALUE_SIZE) ? VALUE_SIZE : buff_len;
	strncpy(value, node->value.value, size);

	return 0;
}

int get_left_count(TREE_NODE *node) 
{
	if (!node) {
		return -1;
	}

	return node->left_count;
}

int get_right_count(TREE_NODE *node)
{
	if (!node) {
		return -1;
	}

	return node->right_count;
}

void print_info(BINARY_TREE *tree, void (*print_func)(TREE_NODE *))
{
	if (!tree) {
		return;
	}

	traverse_tree(INORDER, tree->root, print_func);
}

