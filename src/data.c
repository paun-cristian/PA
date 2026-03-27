#include "data.h"
#include <math.h>
#include <stdlib.h>

int N = 1;


void read_data(int argc, char *argv[]) {
	if (argc < 3)
		exit(-1);

	NODE *head = create_node();
	NODE *current = head;
	FILE *data_in = fopen(argv[1], "rt");
	FILE* data_out = fopen(argv[2], "wt");

	if (data_in == NULL) {
		free_list(head);
		fclose(data_out);
		exit(-1);
	}
	if (data_out == NULL) {
		free_list(head);
		fclose(data_in);
		exit(-1);
	}

	if (fscanf(data_in, "%d %lf", &N, &head->valoare) != 2) {
		free_list(head);
		exit(-1);
	}
	head->calculate_randament = calculare_randament;

	for (int i = 1; i < N; ++i) {
		NODE *aux = create_node();
		if (fscanf(data_in, "%lf", &aux->valoare) != 1)
			break;
		aux->calculate_randament = calculare_randament;
		current->next = aux;
		current = aux;
	}
	compute_randament(head);
	print_stats(head, data_out);
	free_list(head);
	fclose(data_in);
	fclose(data_out);
}

double calculare_randament(NODE* node_k, NODE* node_kminus) {
	double randament = (node_k->valoare - node_kminus->valoare) / node_kminus->valoare;
	return randament;
}

void compute_randament(NODE* head) {
	if (!head || !head->next)
		return;

	for (NODE* aux = head; aux->next != NULL; aux = aux->next) {
		aux->next->randament = aux->calculate_randament(aux->next, aux);
	}

}

double compute_average(NODE *head) {
	if (!head || !head->next)
		exit(-1);
	double sum = 0.0;
	
	for (NODE* aux = head->next; aux != NULL; aux = aux->next)
		sum += aux->randament;
	sum /= (N - 1);

	return sum;
}

double compute_volatility(NODE *head, double average) {
	if (!head || !head->next)
		exit(-1);
	double volatility = 0.0;

	for (NODE* aux = head->next; aux != NULL; aux = aux->next)
		volatility += (aux->randament - average) * (aux->randament - average);

	volatility = sqrt(volatility / (N - 1));

	return volatility;
}

void print_list(NODE* head) {
	NODE* current = head;
	while (current) {
		current = current->next;
	}
}

void print_stats(NODE* head, FILE *data_out) {
	double average = compute_average(head);
	double volatility = compute_volatility(head, average);
	double sharpe_ratio = average / volatility;
	volatility = ((double)((int)(volatility * 1000))) / 1000;
	average = ((double)((int)(average * 1000))) / 1000;
	sharpe_ratio = ((double)(int)(sharpe_ratio * 1000)) / 1000;
	
	fprintf(data_out, "%.3f\n", average);
	fprintf(data_out, "%.3f\n", volatility);
	fprintf(data_out, "%.3f\n", sharpe_ratio);
}