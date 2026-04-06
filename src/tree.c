#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tree.h"

#define MAX_BUFFER_SIZE 512

TREENODE* create_tree_node() {
	TREENODE* new = (TREENODE* )malloc(sizeof(TREENODE));
	if (!new)
		exit(-1);

	new->left = NULL;
	new->right = NULL;
	new->depth = 0; // randul de pe care se citeste

	return new;
}
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
	while (fgets(buffer, MAX_BUFFER_SIZE, data_in) != NULL) {
		if (isdigit(buffer[0]))
			insert_symbols(buffer, root);
		else {
			
		}
	}
}

void insert_symbols(char buffer[], TREENODE* root) {
	char *p = strtok(buffer, ",");
	if (!p)
		exit(-1);

	int stock_nr = 0;
	while (p) {
		strcpy(root->stocks[stock_nr++].symbol, p);  
		p = strtok(NULL, ",");
	}
}

void free_tree(TREENODE* root);

