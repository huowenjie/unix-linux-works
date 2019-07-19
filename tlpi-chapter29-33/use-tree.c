#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "binary-tree.h"

void print_tree(TREE_NODE *node)
{
	char key[32]   = { 0 };
	char value[32] = { 0 };
	int  left  = 0;
	int  right = 0;

	get_key(node, key, 32);
	get_value(node, value, 32);
	left  = get_left_count(node);
	right = get_right_count(node);

	printf("tree--key = %s, value = %s, left count = %d, right count = %d\n", 
		key, value, left, right);
}

int main(int argc, char *argv[])
{
	BINARY_TREE *tree = initialize();
	char buff[128] = { 0 };

	add(tree, "A", "bcd");
	add(tree, "D", "abc");
	add(tree, "H", "cqy");
	add(tree, "F", "hwjcqy");
	add(tree, "J", "hwjcqy");
	add(tree, "K", "hwjcqy");
	add(tree, "I", "hwjcqy");
	del(tree, "J");

	print_info(tree, print_tree);

	if (terminate(tree) < 0) {
		printf("terminate tree failed!\n");
	}

	return 0;
}

