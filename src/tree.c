#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tree.h"

TREENODE* create_tree_node() {
	TREENODE* new = (TREENODE* )malloc(sizeof(TREENODE));
	if (!new)
		exit(-1);

	new->left = NULL;
	new->right = NULL;
	new->stock_no = 0;
	new->stocks = (StockList* )malloc(MAX_SYMS * sizeof(StockList));
	if (!new->stocks)
		exit(-1);

	return new;
}
/*
Introduce in a char[] the order of node placing, as in
lrl, lll, rrr etc.
we know that for a node k the number of movements for it is the number of days
we reduce the time complexity from O(n^2) - iteration on values[][]
to O(n) - iteration through char[] and building the tree
so first 10 chars will be for day 1
next 10 for day 2
and so on
*/
void create_tree(int argc, char *argv[]) {
	TREENODE* root = create_tree_node();

	if (argc < 3)
		exit(-1);

	FILE *data_in = fopen(argv[1], "rt");
	FILE* data_out = fopen(argv[2], "wt");
		
	if (data_in == NULL) {
		fclose(data_out);
		exit(-1);
	}
	if (data_out == NULL) {
		fclose(data_in);
		exit(-1);
	}
	char buffer[MAX_BUFFER_SIZE];
	char movements[MAX_ACTIONS * MAX_DAYS];
	int action_nr = 0, days = 0;
	float prev_day[MAX_DAYS], curr_day[MAX_DAYS];

	while (fgets(buffer, MAX_BUFFER_SIZE, data_in) != NULL) {
		if (!isdigit(buffer[0])) {
			insert_symbols(buffer, root);
		}
		else {
			insert_values(buffer, movements, prev_day, curr_day, action_nr, days);
			days++;
		}
	}

	for (int i = 0; i < MAX_SYMS; ++i)
		disperse_actions(root, movements, days, 0, i);
	
}

void insert_symbols(char buffer[], TREENODE* root) {
	char *p = strtok(buffer, ",");
	if (!p)
		exit(-1);

	int action_nr = 0;
	while (p) {
		strcpy(root->stocks[action_nr++].symbol, p);  
		p = strtok(NULL, ",");
	}
}

void insert_values(char buffer[], char movements[], double prev_day[],
				   double curr_day[], int action_nr, int day) {

	char *p = strtok(buffer, ",");
	if (!p)
		exit(-1);

	if (day == 0) {
		int stock_nr = 0;
		while (p) {
			curr_day[stock_nr++] = atof(p);
			p = strtok(NULL, ",");
		}
	}
	else {
		int stock_nr = 0;
		while (p) {
			prev_day[stock_nr] = curr_day[stock_nr];
			curr_day[stock_nr] = atof(p);
			if (curr_day[stock_nr] > prev_day[stock_nr]) { //right
				movements[stock_nr + (day - 1) * MAX_ACTIONS] = 'r';
			}
			else { // left
				movements[stock_nr + (day - 1) * MAX_ACTIONS] = 'l';
			}
			stock_nr++;
			p = strtok(NULL, ",");
		}
	}
}

// building the tree
void disperse_actions(TREENODE* root, char movements[],
					  int days, int curr_day, int pos) {
	if (curr_day >= days - 1)
		return;
	
	if (movements[curr_day * MAX_ACTIONS + pos] == 'r') {
		if (root->right == NULL)
			root->right = create_tree_node();
		int curr_node_stock_no = root->right->stock_no++;

		strcpy(root->right->stocks[curr_node_stock_no].symbol,
			   root->stocks[pos].symbol);
		disperse_actions(root->right, movements, days,
						 curr_day + 1, pos);
	}
	else {
		if (root->left == NULL)
			root->left = create_tree_node();
		int curr_node_stock_no = root->left->stock_no++;

		strcpy(root->left->stocks[curr_node_stock_no].symbol,
			   root->stocks[pos].symbol);
		disperse_actions(root->left, movements, days,
						 curr_day + 1, pos);
	}
}
// int day_count = 0;

// for (int i = 0; i < days; ++i) {
// 	if (i % MAX_ACTIONS == 0)
// 		day_count++;

// 	if (movements[MAX_ACTIONS * day_count + i] == 'r')
// 		root->
// }

void show_matches(TREENODE* root) {
	if (!root->left || !root->right)
		return;

	
}
void free_tree(TREENODE* root);
