#include <stdio.h>
#include <time.h>

#include "pcg_basic.h"
#include "genom.h"
#include "data_get.h"
#include "agent_movement.h"
#include "genetic.h"

// [from, to]
void shuffle(generation *g, int from, int to) {
    int i = from;
    pcg32_random_t rng;
    pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&(i));
    while (i < to - 1) {
        int k = (int)pcg32_boundedrand_r(&rng, to - i) + i + 1;
        genom temp = g->gen[i];
        *(g->gen + i) = *(g->gen + k);
        *(g->gen + k) = temp;
        i++;
    }
}

void selection(generation *g, double whole_size_ratio, double best_size_ratio) {
    int new_gen_size = g->max_size*whole_size_ratio;
    if (new_gen_size < g->actual_size) {
        int new_best_size = best_size_ratio*new_gen_size;
        shuffle(g, new_best_size - 1, g->actual_size - 1);
        g->actual_size = new_gen_size;
    }
}

/*
// Search space functions
*/
void swap_2(genom *g, pcg32_random_t *rng) {
    int i = (int)pcg32_boundedrand_r(rng, g->size);
    int k = (int)pcg32_boundedrand_r(rng, g->size);
    char temp = g->path[i];
    g->path[i] = g->path[k];
    g->path[k] = temp;
}

void swap_3(genom *g, pcg32_random_t *rng) {
    int i = (int)pcg32_boundedrand_r(rng, g->size);
    int k = (int)pcg32_boundedrand_r(rng, g->size);
    int j = (int)pcg32_boundedrand_r(rng, g->size);

    char temp = g->path[i];
    g->path[i] = g->path[k];
    g->path[k] = g->path[j];
    g->path[j] = temp;
}

double uniform(pcg32_random_t *rng) {
    int max_prec = 1000000;
    int numerator = (int)pcg32_boundedrand_r(rng, max_prec + 1);

    return (double)numerator/max_prec;
}

void cut(genom *g, pcg32_random_t *rng) {
    double c = 2.0*uniform(rng);
    double p = 1.0;
    double prob = 0.5;

    while (g->size > 1) {
        if (uniform(rng) <= p) {
            int i = (int)pcg32_boundedrand_r(rng, g->size);
            for (int k=i+1; k<g->size; k++) {
                g->path[k - 1] = g->path[k];
            }
            g->size--;

            p *= prob;
            p *= c;
            c = 2.0*uniform(rng);
        } else {
            break;
        }
    }
}

void insert_random(genom *g, pcg32_random_t *rng, int max_size) {
    double c = 2.0*uniform(rng);
    double p = 1.0;
    double prob = 0.5;

    char moves[4] = {'U', 'D', 'L', 'R'};
    while (g->size < max_size) {
        if (uniform(rng) <= p) {
            g->size++;
            int i = (int)pcg32_boundedrand_r(rng, g->size);
            for (int k=g->size-1; k>i; k--) {
                g->path[k] = g->path[k - 1];
            }
            g->path[i] = moves[(int)pcg32_boundedrand_r(rng, 4)];

            p *= prob;
            p *= c;
            c = 2.0*uniform(rng);
        } else {
            break;
        }
    }
}

void insert_left(genom *g, pcg32_random_t *rng, int max_size) {
    double c = 2.0*uniform(rng);
    double p = 1.0;
    double prob = 0.5;

    char moves[4] = {'U', 'D', 'L', 'R'};
    while (g->size < max_size) {
        if (uniform(rng) <= p) {
            g->size++;
            int i = 0;
            for (int k=g->size-1; k>i; k--) {
                g->path[k] = g->path[k - 1];
            }
            g->path[i] = moves[(int)pcg32_boundedrand_r(rng, 4)];

            p *= prob;
            p *= c;
            c = 2.0*uniform(rng);
        } else {
            break;
        }
    }
}

void insert_right(genom *g, pcg32_random_t *rng, int max_size) {
    double c = 2.0*uniform(rng);
    double p = 1.0;
    double prob = 0.5;

    char moves[4] = {'U', 'D', 'L', 'R'};
    while (g->size < max_size) {
        if (uniform(rng) <= p) {
            g->path[g->size] = moves[(int)pcg32_boundedrand_r(rng, 4)];
            g->size++;

            p *= prob;
            p *= c;
            c = 2.0*uniform(rng);
        } else {
            break;
        }
    }
}

void change_some(genom *g, pcg32_random_t *rng) {
    double c = 2.0*uniform(rng);
    double p = 1.0;
    double prob = 0.5;

    char moves[4] = {'U', 'D', 'L', 'R'};
    while (uniform(rng) <= p) {
        int i = (int)pcg32_boundedrand_r(rng, g->size);
        g->path[i] = moves[(int)pcg32_boundedrand_r(rng, 4)];

        p *= prob;
        p *= c;
        c = 2.0*uniform(rng);
    }
}

void insert_random_path_len(genom *g, pcg32_random_t *rng, int max_size) {
    int new_size = (int)pcg32_boundedrand_r(rng, g->size) + 1;
    char moves[4] = {'U', 'D', 'L', 'R'};
    for (int i=0; i<new_size; i++) {
        if (g->size < max_size) {
            g->size++;
            int i = (int)pcg32_boundedrand_r(rng, g->size);
            for (int k=g->size-1; k>i; k--) {
                g->path[k] = g->path[k - 1];
            }
            g->path[i] = moves[(int)pcg32_boundedrand_r(rng, 4)];
        } else {
            return;
        }
    }
}

void path_copy(genom *g_take, genom *g_put) {
    g_put->did_exit = g_take->did_exit;
    g_put->size = g_take->size;

    for (int i=0; i<g_put->size; i++) {
        g_put->path[i] = g_take->path[i];
    }
}

void mutate(generation *g, stdin_data *data, int probability) {
    pcg32_random_t rng;
    pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&(probability));
    int tres = g->actual_size;
    for (int i=0; i<tres; i++) {
        if ((int)pcg32_boundedrand_r(&rng, 1001) <= probability) {
            genom *genom_to_change = (g->gen + i);

            int search_space = (int)pcg32_boundedrand_r(&rng, 8);
            if (search_space == 0 && genom_to_change->size >= 2) {
                swap_2(genom_to_change, &rng);
            } else if (search_space == 1 && genom_to_change->size >= 3) {
                swap_3(genom_to_change, &rng);
            } else if (search_space == 2) {
                cut(genom_to_change, &rng);
            } else if (search_space == 3 && genom_to_change->size >= 1) {
                insert_random(genom_to_change, &rng, g->max_genom_size);
            } else if (search_space == 4 && genom_to_change->size >= 1) {
                insert_right(genom_to_change, &rng, g->max_genom_size);
            } else if (search_space == 5 && genom_to_change->size >= 1) {
                insert_left(genom_to_change, &rng, g->max_genom_size);
            } else if (search_space == 6 && genom_to_change->size >= 1) {
                change_some(genom_to_change, &rng);
            } else if (search_space == 7 && genom_to_change->size >= 1) {
                insert_random_path_len(genom_to_change, &rng, g->max_genom_size);
            }

            traverse(genom_to_change, data);
        }
    }
}

void make_child(generation *g, stdin_data *data, pcg32_random_t *rng, int p1, int p2) {
    if (g->actual_size < g->max_size) {
        genom *genom_to_change = (g->gen + g->actual_size);
        g->actual_size++;

        int pivot_1 = (int)pcg32_boundedrand_r(rng, (g->gen + p1)->size);
        int pivot_2 = (int)pcg32_boundedrand_r(rng, (g->gen + p2)->size);
        if (pivot_1 + 1 + ((g->gen + p2)->size - pivot_2) < g->max_genom_size) {
            genom_to_change->size = pivot_1 + 1 + ((g->gen + p2)->size - pivot_2);
            int counter = pivot_1 + 1;
            for (int i=0; i<=pivot_1; i++) {
                genom_to_change->path[i] = (g->gen + p1)->path[i];
            }
            for (int i=pivot_2; i<(g->gen + p2)->size; i++) {
                genom_to_change->path[counter] = (g->gen + p2)->path[i];
                counter++;
            }

            traverse(genom_to_change, data);
        }
    } else {
        genom *genom_to_change = (g->gen + p1);

        int pivot_1 = (int)pcg32_boundedrand_r(rng, (g->gen + p1)->size);
        int pivot_2 = (int)pcg32_boundedrand_r(rng, (g->gen + p2)->size);
        if (pivot_1 + 1 + ((g->gen + p2)->size - pivot_2) < g->max_genom_size) {
            genom_to_change->size = pivot_1 + 1 + ((g->gen + p2)->size - pivot_2);
            int counter = pivot_1 + 1;

            for (int i=pivot_2; i<(g->gen + p2)->size; i++) {
                genom_to_change->path[counter] = (g->gen + p2)->path[i];
                counter++;
            }

            traverse(genom_to_change, data);
        }
    }
}

void reproduce(generation *g, stdin_data *data, int probability) {
    if (g->actual_size >= 2) {
        pcg32_random_t rng;
        pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&(probability));

        int pair[2];
        int ctr = 0;
        shuffle(g, 0, g->actual_size - 1);

        for (int i=0; i<g->actual_size; i++) {
            if ((int)pcg32_boundedrand_r(&rng, 1001) <= probability) {
                pair[ctr] = i;
                ctr++;
            }

            if (ctr >= 2) {
                if ((g->gen + pair[0])->size > 2) {
                    if ((g->gen + pair[1])->size > 2) {
                        make_child(g, data, &rng, pair[0], pair[1]);
                        ctr = 0;
                    } else {
                        ctr = 1;
                    }
                } else {
                    pair[0] = pair[1];
                    ctr = 1;
                }
            }
        }
    }
}
