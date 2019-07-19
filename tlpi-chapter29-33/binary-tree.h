#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include <stddef.h>

/**
 * 不平衡二叉树
 * 要求：
 * 1.线程安全
 * 2.初始化、增、删、改、查、清理
 * 3.无需维护平衡二叉树
 * 4.每个节点需要保存一个键值对
 * 5.树中每个节点的定义一个数据结构，其中应该
 *   包含互斥量
 */

typedef struct value_st TREE_VALUE;		// 二叉树节点数据域
typedef struct node_st  TREE_NODE;		// 二叉树节点
typedef struct tree_st  BINARY_TREE;	// 二叉树对象

/**
 * int 类型的返回值如无特殊说明：0 代表成功，-1 代表失败
 */

// 生成一颗二叉树，返回其结构首地址
BINARY_TREE * initialize();

// 释放二叉树对象
int terminate(BINARY_TREE *tree);

// 向树中添加删除查找元素
int add(BINARY_TREE *tree, 	  const char *key, const char *value);
int del(BINARY_TREE *tree,    const char *key);
int lookup(BINARY_TREE *tree, const char *key, char *value, size_t buff_len);

// 获取树信息
int get_key(TREE_NODE *node,   char *key,   size_t buff_len);
int get_value(TREE_NODE *node, char *value, size_t buff_len);
int get_left_count(TREE_NODE *node);
int get_right_count(TREE_NODE *node);

// 打印树信息
void print_info(BINARY_TREE *tree, void (*print_func)(TREE_NODE *));

#endif //__BINARY_TREE_H__

