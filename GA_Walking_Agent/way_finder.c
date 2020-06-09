#include <stdio.h>
#include <stdlib.h>

#include "way_finder.h"
#include "genetic.h"
#include "agent_movement.h"

int compare_genoms(genom *g_1, genom *g_2) {
    if (g_1->size == 0) {
        if (g_2->size != 0) {
            return 1;
        } else {
            return 0;
        }
    } else if (g_2->size == 0) {
        if (g_1->size != 0) {
            return -1;
        } else {
            return 0;
        }
    } else {
        if (g_1->did_exit) {
            if (g_2->did_exit) {
                if (g_1->size > g_2->size) {
                    return 1;
                } else if (g_1->size < g_2->size) {
                    return -1;
                } else {
                    return 0;
                }
            } else {
                return -1;
            }
        } else {
            if (g_2->did_exit) {
                return 1;
            } else {
                if (g_1->size > g_2->size) {
                    return 1;
                } else if (g_1->size < g_2->size) {
                    return -1;
                } else {
                    return 0;
                }
            }
        }
    }
}

void insertion_sort_of_generation(generation *g) {
    for (int i=1; i<g->actual_size; i++) {
        genom key = *(g->gen + i);
        int j = i - 1;

        while (j >= 0 && compare_genoms((g->gen + j), &key) > 0) {
            *(g->gen + j + 1) = *(g->gen + j);
            j = j - 1;
        }
        *(g->gen + j + 1) = key;
    }

    int counter = g->actual_size-1;
    while (counter >= 0 && g->gen[counter].size == 0) {
        g->actual_size--;
        counter--;
    }
}

genom * genom_cpy(genom *g) {
    genom *new_g = malloc(sizeof(genom));
    new_g->size = g->size;
    new_g->did_exit = g->did_exit;
    new_g->path = malloc(sizeof(char)*new_g->size);
    for (int i=0; i<new_g->size; i++) {
        new_g->path[i] = g->path[i];
    }

    return new_g;
}

void find_new_path_to_exit(generation *g, stdin_data *data, clock_t time_start) {
    genom *g_to_change;
    if (g->actual_size < g->max_size) {
        g_to_change = (g->gen + g->actual_size);
        g->actual_size++;
    } else {
        g_to_change = (g->gen + g->actual_size - 1);
    }
    bool did_exit = false;
    int rep_treshold = 10;
    int rep_ctr = 0;
    while (rep_ctr < rep_treshold && !did_exit && (double)(clock() - time_start)/CLOCKS_PER_SEC < (double)data->max_time) {
        did_exit = random_moves(g_to_change, data, g->max_genom_size);
        rep_ctr++;
    }
}

void print_best_path(genom *g, stdin_data *data) {
    int agent_row = data->agent_row;
    int agent_col = data->agent_col;
    for (int i=0; i<g->size; i++) {
        move(g->path[i], &agent_row, &agent_col);
        data->labirynth[agent_row][agent_col] = 'T';
    }

    for (int i=0; i<data->row; i++) {
        printf("%s\n", data->labirynth[i]);
    }
}

int min_2(int a, int b) {
    if (a <= b) {
        return a;
    } else {
        return b;
    }
}

genom * find_way(stdin_data *data, generation *g, clock_t time_start) {

    while (g->actual_size < 0.3*g->max_size) {
        find_new_path_to_exit(g, data, time_start);
    }
    insertion_sort_of_generation(g);
    genom *best_genom = genom_cpy(g->gen);

    int reset_treshold = best_genom->size*min_2(data->col, data->row);
    int reset_ctr = 0;
    int no_progress_treshold = reset_treshold*10;
    int no_progress_ctr = 0;

    while ((double)(clock() - time_start)/CLOCKS_PER_SEC < (double)data->max_time) {
        if (reset_ctr >= reset_treshold) {
            if (g->actual_size > 0.15*g->max_size) {
                g->actual_size = 0.15*g->max_size;
            }

            while (g->actual_size < 0.3*(g->max_size)) {
                find_new_path_to_exit(g, data, time_start);
            }

            (g->gen + g->actual_size)->size = best_genom->size;
            (g->gen + g->actual_size)->did_exit = best_genom->did_exit;
            for (int i=0; i<best_genom->size; i++) {
                (g->gen + g->actual_size)->path[i] = best_genom->path[i];
            }
            g->actual_size++;

            reset_ctr = 0;
            goto SORT;
        }

        if (g->actual_size >= 0.75*g->max_size) {
            selection(g, 0.3, 0.5);
        }
        reproduce(g, data, 850);
        mutate(g, data, 150);

SORT:   insertion_sort_of_generation(g);
        if (compare_genoms(g->gen, best_genom) < 0) {
            free(best_genom->path);
            free(best_genom);
            best_genom = genom_cpy(g->gen);
            reset_treshold = best_genom->size*min_2(data->col, data->row);
            reset_ctr = 0;
            no_progress_treshold = 5*reset_treshold;
            no_progress_ctr = 0;
        } else {
            reset_ctr++;
            no_progress_ctr++;
            if (no_progress_ctr >= no_progress_treshold) {
                break;
            }
        }
    }

    print_best_path(best_genom, data);

    return best_genom;
}
