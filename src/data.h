#include "lists.h"
#include <stdio.h>

void read_data(int argc, char *argv[]);
double calculare_randament(NODE* node_k, NODE* node_kminus);
double compute_average(NODE *head);
double compute_volatility(NODE *head, double average);
void print_list(NODE* head);
void compute_randament(NODE* head);
void print_stats(NODE* head, FILE *data_out);