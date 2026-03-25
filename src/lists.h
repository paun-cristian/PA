#ifndef LISTS_H
#define LISTS_H

typedef struct NODE {
    double valoare;
    double randament;
    struct NODE* next;
    double (*calculate_randament)(struct NODE* , struct NODE* );
} NODE;

NODE* create_node();
NODE* create_list();
void free_list(NODE* head);
void show_list(NODE* head);

#endif

