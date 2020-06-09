#include <stdio.h>
#include <stdlib.h>

#include "way_finder.h"

/*
// List to take first 
*/
typedef struct list list;
struct list {
    list *next;
    char c;
};

genom * take_first_solution(int max_size) {
    list *l = NULL;
    list *tail = NULL;
    char c = (char)fgetc(stdin);
    int size = 0;
    while (c != EOF) {
        if (c != '\n') {
            list *new_el = malloc(sizeof(list));
            new_el->c = c;
            new_el->next = NULL;
            if (l == NULL) {
                l = new_el;
            } else {
                tail->next = new_el;
            }
            tail = new_el;
            size++;
        }
        c = (char)fgetc(stdin);
    }

    genom *new_g;

    if (size > max_size) {
        new_g = NULL;

        list *temp_last;
        for (int i=0; i<size; i++) {
            temp_last = l;
            l = l->next;
            free(temp_last);
        }
    } else {
        new_g = malloc(sizeof(genom));

        new_g->path = malloc(sizeof(char)*max_size);
        new_g->size = size;
        new_g->did_exit = true;

        list *temp_last;
        for (int i=0; i<size; i++) {
            new_g->path[i] = l->c;
            temp_last = l;
            l = l->next;
            free(temp_last);
        }
    }

    return new_g;
}

int main() {
    clock_t time_start = clock();
    stdin_data *data = take_stdin();
    genom *first_solution = take_first_solution((data->col-2)*(data->row-2));

    generation *g = malloc(sizeof(generation));
    if (first_solution != NULL) {
        g->actual_size = 1;
        g->max_genom_size = (data->col-2)*(data->row-2);
        g->max_size = 100;
        g->gen = malloc(sizeof(genom)*g->max_size);
        *(g->gen) = *(first_solution);
        free(first_solution);

        for (int i=1; i<g->max_size; i++) {
            (g->gen + i)->path = malloc(sizeof(char)*g->max_genom_size);
            (g->gen + i)->size = 0;
            (g->gen + i)->did_exit = false;
        }
    } else {
        g->actual_size = 0;
        g->max_genom_size = (data->col-2)*(data->row-2);
        g->max_size = 100;
        g->gen = malloc(sizeof(genom)*g->max_size);

        for (int i=0; i<g->max_size; i++) {
            (g->gen + i)->path = malloc(sizeof(char)*g->max_genom_size);
            (g->gen + i)->size = 0;
            (g->gen + i)->did_exit = false;
        }
    }

    genom *best_genom = find_way(data, g, time_start);
    printf("%d\n", best_genom->size);
    for (int i=0; i<best_genom->size; i++) {
        fprintf(stderr, "%c", best_genom->path[i]);
    }
}
