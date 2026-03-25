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

	if (data_in == NULL || data_out == NULL) {
		free_list(head);
		fclose(data_in);
		fclose(data_out);
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
	double randament = node_k->valoare / node_kminus->valoare - 1;
	randament *= 1000;
	randament = (double)((int)randament);

	return randament / 1000;
}

void compute_randament(NODE* head) {
	if (!head || !head->next)
		return;

	for (NODE* aux = head; aux->next != NULL; aux = aux->next) {
		aux->next->randament = aux->calculate_randament(aux->next, aux);
	}

}

double compute_average(NODE *head) {
	double sum = 0;
	
	for (NODE* aux = head->next; aux != NULL; aux = aux->next)
		sum += aux->randament;

	return sum / N;
}

double compute_volatility(NODE *head, double average) {
	double volatility = 0;

	for (NODE* aux = head->next; aux != NULL; aux = aux->next)
		volatility += (aux->randament - average) * (aux->randament - average);

	volatility = sqrt(volatility / N);

	return volatility;
}

void print_list(NODE* head) {
	NODE* current = head;
	while (current) {
		printf("Valoare: %f, rand: %.3f \n", current->valoare, current->randament);
		current = current->next;
	}
}

void print_stats(NODE* head, FILE *data_out) {
	double average = compute_average(head);
	double volatility = compute_volatility(head, average);
	fprintf(data_out, "%.3f\n", average);
	fprintf(data_out, "%.3f\n", volatility);
	fprintf(data_out, "%.3f\n", average / volatility);
}