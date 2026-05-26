#include <stdio.h>
#include <stdlib.h>
#include "markov.h"

static unsigned gcd(unsigned a, unsigned b) {
	while (b != 0) {
		unsigned temp = b;
		b = a % b;
		a = temp;
	}
	return a < 0 ? -a : a;
}

static Fraction simplify(Fraction f) {
	if (f.num == 0) {
		f.den = 1;
		return f;
	}

	unsigned g = gcd(f.num, f.den);
	f.num /= g;
	f.den /= g;
	if (f.den < 0) {
		f.num = -f.num;
		f.den = -f.den;
	}

	return f;
}

static Fraction add_fractions(Fraction a, Fraction b) {
	if (a.num == 0) return b;
	if (b.num == 0) return a;
	Fraction res = {0, 0};
	unsigned g = gcd(a.den, b.den);
	unsigned lcm = (a.den / g) * b.den;
	res.den = lcm;
	res.num = a.num * (lcm / a.den) + b.num * (lcm / b.den);

	return simplify(res);
}

static Fraction mul_fractions(Fraction a, Fraction b) {
	Fraction res;
	res.num = a.num * b.num;
	res.den = a.den * b.den;
	return simplify(res);
}

// Creeaza o matrice de adiacenta pentru graful Markov
MARKOV_MATRIX* create_markov_matrix(size_t element_count) {
	MARKOV_MATRIX* matrix = (MARKOV_MATRIX* )calloc(element_count, sizeof(MARKOV_MATRIX));
	if (!matrix)
		exit(-1);

	for (size_t i = 0; i < element_count; i++) {
		matrix[i].node_count = 0;
		matrix[i].adjacent_nodes = calloc(element_count, sizeof(int));
		if (!matrix[i].adjacent_nodes) {
			exit(-1);
		}
	}

	return matrix;
}

// Calculeaza sau preia id-ul asociat unui nod
static int get_mapped_id(int raw_node, int* mapping, int* mapped_count) {
	for (int i = 0; i < *mapped_count; i++) {
		if (mapping[i] == raw_node) {
			return i;
		}
	}
	mapping[*mapped_count] = raw_node;
	return (*mapped_count)++;
}

// Construieste graful si simuleaza tranzitiile probabilitatilor
void create_markov_graph(int argc, const char* argv[]) {
	if (argc < 3) exit(-1);
	
	FILE *data_in = fopen(argv[1], "rt");
	FILE *data_out = fopen(argv[2], "wt");
	
	if (!data_in || !data_out)
		exit(-1);
	
	size_t observations = 0;
	float interval_size = 0.0;
	int days = 0;
	float p_start = 0.0, p_target = 0.0;
	
	if (fscanf(data_in, "%lu", &observations) != 1)
		exit(-1);

	size_t max_mapping_size = observations * 2 + 2; // if N observations are unique, we double (the + 2 are for start and target)

	fscanf(data_in, "%f", &interval_size);
	fscanf(data_in, "%d", &days);
	fscanf(data_in, "%f", &p_start);
	fscanf(data_in, "%f", &p_target);

	int raw_start = (int)(p_start / interval_size);
	int raw_target = (int)(p_target / interval_size);

	int* mapping = calloc(max_mapping_size, sizeof(int));
	int mapped_count = 0;
	
	// Ensure start and target are mapped
	get_mapped_id(raw_start, mapping, &mapped_count);
	int target_id = get_mapped_id(raw_target, mapping, &mapped_count);

	MARKOV_MATRIX* matrix = create_markov_matrix(max_mapping_size);

	float from_interval = 0.0, to_interval = 0.0;
	fscanf(data_in, "%f", &from_interval);
	int first_id = get_mapped_id((int)(from_interval / interval_size), mapping, &mapped_count);

	for (int i = 1; i < observations; ++i) {
		fscanf(data_in, "%f", &to_interval);
		int raw_from = (int)(from_interval / interval_size);
		int raw_to = (int)(to_interval / interval_size);

		int from_id = get_mapped_id(raw_from, mapping, &mapped_count);
		int to_id = get_mapped_id(raw_to, mapping, &mapped_count);

		matrix[from_id].adjacent_nodes[matrix[from_id].node_count++] = to_id;
		from_interval = to_interval;	
	}

	Fraction* prev_day = calloc(mapped_count, sizeof(Fraction));
	Fraction* curr_day = calloc(mapped_count, sizeof(Fraction));
	
	for (int i = 0; i < mapped_count; i++)
		prev_day[i] = (Fraction){0, 1};

	prev_day[first_id] = (Fraction){1, 1};

	for (int d = 0; d < days; ++d) {
		// Output probability before transition
		if (prev_day[target_id].num == 0) {
			fprintf(data_out, d == days - 1 ? "0" : "0\n");
		} else if (prev_day[target_id].den == 1) {
			fprintf(data_out, d == days - 1 ? "%d" : "%d\n", prev_day[target_id].num);
		} else {
			fprintf(data_out, d == days - 1 ? "%d/%d" : "%d/%d\n", prev_day[target_id].num, prev_day[target_id].den);
		}

		for (int i = 0; i < mapped_count; i++)
			curr_day[i] = (Fraction){0, 1};

		for (int u = 0; u < mapped_count; u++) {
			if (prev_day[u].num > 0) {
				int out_deg = matrix[u].node_count;
				if (out_deg == 0) continue;

				Fraction prob_trans = {1, out_deg};
				prob_trans = simplify(prob_trans);
				Fraction prob_flow = mul_fractions(prev_day[u], prob_trans);

				for (int i = 0; i < out_deg; i++) { // taking all adjacent nodes
					int v = matrix[u].adjacent_nodes[i];
					curr_day[v] = add_fractions(curr_day[v], prob_flow);
				}
			}
		}

		for (int i = 0; i < mapped_count; i++) {
			prev_day[i] = curr_day[i];
		}
	}

	for (int i = 0; i < max_mapping_size; i++) {
		free(matrix[i].adjacent_nodes);
	}

	free(matrix);
	free(mapping);
	free(prev_day);
	free(curr_day);
	
	fclose(data_in);
	fclose(data_out);
}