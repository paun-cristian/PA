#pragma once

#define MAX_SYM_LEN 5
#define MAX_SYMS 20

/*
	Move n stocks according to their increase/decrease
	Create an array to day k value for each action
	Move the action array dependent on their price
	!No need to stock the actual value in the tree
	!No need to have a StockList* next field
	!No need for depth field, take the symetrical nodes ?
	At last, BFS + analyzing the order in the root to print
	as asked
*/

typedef struct StockList {
	char symbol[MAX_SYM_LEN];
} StockList;

typedef struct TREENODE {
	StockList* stocks;
	struct TREENODE* left;
	struct TREENODE* right;
	int depth;
} TREENODE;

TREENODE* create_tree_node();
void insert_symbols(char buffer[], TREENODE* root);
void create_tree(int argc, char *argv[]);
void free_tree(TREENODE* root);
