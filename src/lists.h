#pragma once

#define MAX_CITY_LEN 30
typedef struct NODE
{
	double valoare;
	double randament;
	struct NODE *next;
	double (*calculate_randament)(struct NODE *, struct NODE *);
} NODE;

typedef struct CITY_NODE {
	char *city;
	NODE* next;
} CITY_NODE;


NODE *create_node();
NODE *create_list();
CITY_NODE *create_city_node();
int stack_height(NODE* top);
double knode_value(NODE* top, int pos);
void free_list(NODE *head);
void show_list(NODE *head);
void push(CITY_NODE* top_city, double value);
void free_city(CITY_NODE* city);
void free_cities(CITY_NODE** cities);

