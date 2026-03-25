#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "lists.h"


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


NODE* create_list() {
	int n;
	if (scanf("%d", &n) != 1 || n <= 0)
		return NULL;

	NODE* head = create_node();
	if (scanf("%lf", &head->valoare) != 1)
		return head;

	NODE* current = head;
	for (int i = 1; i < n; i++) {
		NODE* node = create_node();
		if (scanf("%lf", &node->valoare) != 1)
			break;
		current->next = node;
		current = node;
	}

	return head;
}


void free_list(NODE* head) {
	while (head != NULL) {
		NODE* next = head->next;
		free(head);
		head = next;
	}
}


void show_list(NODE* head) {
	while (head != NULL) {
		printf("%lf ", head->valoare);
		head = head->next;
	}
	printf("\n");

}