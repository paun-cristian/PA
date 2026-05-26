#include "data.h"
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int N = 1;

// Citeste datele si initializeaza lista de valori
void read_data(int argc, const char *argv[]) {
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
	NODE *head = create_node();
	NODE *current = head;
		
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

// Calculeaza randamentul dintre doua zile consecutive
double calculare_randament(const NODE* node_k, const NODE* node_kminus) {
	double randament = (node_k->valoare - node_kminus->valoare) / node_kminus->valoare;
	return randament;
}

// Populeaza campul de randament pentru fiecare nod
void compute_randament(NODE* head) {
	if (!head || !head->next)
		return;

	for (NODE* aux = head; aux->next != NULL; aux = aux->next) {
		aux->next->randament = aux->calculate_randament(aux->next, aux);
	}

}

// Calculeaza media randamentelor din lista
double compute_average(NODE *head) {
	if (!head || !head->next)
		exit(-1);
	double sum = 0.0;
	
	for (NODE* aux = head->next; aux != NULL; aux = aux->next)
		sum += aux->randament;
	sum /= (N - 1);

	return sum;
}

// Calculeaza volatilitatea pe baza mediei
double compute_volatility(NODE *head, double average) {
	if (!head || !head->next)
		exit(-1);
	double volatility = 0.0;

	for (NODE* aux = head->next; aux != NULL; aux = aux->next)
		volatility += (aux->randament - average) * (aux->randament - average);

	volatility = sqrt(volatility / (N - 1));

	return volatility;
}

// Afiseaza media, volatilitatea si indicele Sharpe
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

// Cauta si afiseaza oportunitatea dintre orase care nu au evolutie similara
void find_opportunity(int argc, const char *argv[]) {
	if (argc < 3)
		exit(-1);

	FILE* data_in = fopen(argv[1], "rt");
	FILE* data_out = fopen(argv[2], "wt");

	if (data_in == NULL) {
		fclose(data_out);
		exit(-1);
	}
	if (data_out == NULL) {
		fclose(data_in);
		exit(-1);
	}

	CITY_NODE** cities = process_cities(data_in);
	int minimal_stack_height = min_city_height(cities);

	for (int i = 0; i < minimal_stack_height; ++i) {
		double city1_kvalue = knode_value(cities[0]->next, i);
		double city2_kvalue = knode_value(cities[1]->next, i);
		double city3_kvalue = knode_value(cities[2]->next, i);

		if (city1_kvalue == city2_kvalue && city1_kvalue != city3_kvalue) {
			double difference = fabs(city1_kvalue - city3_kvalue);
			fprintf(data_out, "ziua %d - %.2lf - %s", i + 1, difference, cities[2]->city);
		}
		if (city1_kvalue == city3_kvalue && city1_kvalue != city2_kvalue) {
			double difference = fabs(city1_kvalue - city2_kvalue);
			fprintf(data_out, "ziua %d - %.2lf - %s", i + 1, difference, cities[1]->city);
		}
		if (city2_kvalue == city3_kvalue && city1_kvalue != city2_kvalue) {
			double difference = fabs(city1_kvalue - city2_kvalue);
			fprintf(data_out, "ziua %d - %.2lf - %s", i + 1, difference, cities[0]->city);
		}
		printf("\n");
	}

	free_cities(cities);
}

// Creeaza si populeaza stivele de preturi pentru fiecare oras
CITY_NODE** process_cities(FILE* data_in) {
	char *buffer = (char* )malloc(MAX_CITY_LEN * sizeof(char));
	if (buffer == NULL)
		exit(-1);

	CITY_NODE** cities = (CITY_NODE** )malloc(NR_CITIES * sizeof(CITY_NODE* ));
	if (cities == NULL) {
		free(buffer);
		exit(-1);
	}

	int nr_cities = 0;
	CITY_NODE* current_city = NULL;

	while (fgets(buffer, MAX_CITY_LEN, data_in) != NULL) {
		if (!isdigit(buffer[0])) {
			CITY_NODE *aux = create_city_node();
			current_city = aux;
			strcpy(current_city->city, buffer);
			cities[nr_cities++] = current_city;
		}
		else {
			double value = atof(buffer);
			push(current_city, value);
		}
	}

	free(buffer);
	buffer = NULL;
	return cities;
}

// Gaseste si returneaza marimea minima a stivelor de preturi
int min_city_height(CITY_NODE** cities) {
	int minimum_height = stack_height(cities[0]->next);
	for (int i = 1; i < NR_CITIES; ++i) {
		int current_height = stack_height(cities[i]->next);
		if (current_height < minimum_height)
			minimum_height = current_height;
	}

	return minimum_height;
}

int test_num(const char test_name[]) {
	int num = 0;

	for (int i = 0; i < strlen(test_name); ++i) {
		if (isdigit(test_name[i]))
			num = num * 10 + (test_name[i] - '0');
	}

	return num;
}