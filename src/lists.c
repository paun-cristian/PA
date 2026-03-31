#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "lists.h"
#include "data.h"


NODE* create_node() {
	NODE* new = (NODE* )malloc(sizeof(NODE));
	if (new == NULL)
		exit(-1);

	new->valoare = 0;
	new->randament = 0;
	new->next = NULL;
	new->calculate_randament = NULL;

	return new;
}


// NODE* create_list() {
// 	int n;
// 	if (scanf("%d", &n) != 1 || n <= 0)
// 		return NULL;

// 	NODE* head = create_node();
// 	if (scanf("%lf", &head->valoare) != 1)
// 		return head;

// 	NODE* current = head;
// 	for (int i = 1; i < n; i++) {
// 		NODE* node = create_node();
// 		if (scanf("%lf", &node->valoare) != 1)
// 			break;
// 		current->next = node;
// 		current = node;
// 	}

// 	return head;
// }

// void show_list(NODE* head) {
// 	while (head != NULL) {
// 		printf("%lf ", head->valoare);
// 		head = head->next;
// 	}
// 	printf("\n");

// }

void free_list(NODE* head) {
	while (head != NULL) {
		NODE* next = head->next;
		free(head);
		head = NULL;
		head = next;
	}
}


CITY_NODE* create_city_node() {
	CITY_NODE* new = (CITY_NODE* )malloc(sizeof(CITY_NODE));
	if (new == NULL)
		exit(-1);

	new->next = NULL;
	new->city = (char* )malloc(MAX_CITY_LEN * sizeof(char));
	if (new->city == NULL) {
		free(new);
		exit(-1);
	}

	return new;
}
/*
	Basically each city is a "dummy" node that points to the first
value in its stack.
	The goal: make a vector of stacks, with first nodes being the
names of the cities, and each stack top being the node
pointed at bu the CITY_NODE.
*/

void push(CITY_NODE* top_city, double value) {
	NODE* new = create_node();
	new->valoare = value;
	new->next = top_city->next;
	top_city->next = new;
}

void free_city(CITY_NODE* city) {
	NODE* top = city->next;
	free_list(top);
	free(city->city);
	free(city);
}

void free_cities(CITY_NODE** cities) {
	for (int i = 0; i < NR_CITIES; ++i) {
		free_city(cities[i]);
	}
	free(cities);
}

int stack_height(NODE* top) {
	if (top == NULL)
		return 0;
	return 1 + stack_height(top->next);
}

double knode_value(NODE* top, int pos) {
	for (int i = 0; i < pos; ++i)
		top = top->next;

	return top->valoare;
}