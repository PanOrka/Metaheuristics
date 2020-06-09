#include <time.h>
#include <stdio.h>

#include "pcg_basic.h"
#include "genom.h"
#include "data_get.h"
#include "agent_movement.h"

bool check_in_exit(int agent_row, int agent_col, stdin_data *data) {
    return (data->labirynth[agent_row][agent_col] == '8' ||
            data->labirynth[agent_row][agent_col + 1] == '8' ||
            data->labirynth[agent_row][agent_col - 1] == '8' ||
            data->labirynth[agent_row + 1][agent_col] == '8' ||
            data->labirynth[agent_row - 1][agent_col] == '8');
}

bool check_move(char step, int agent_row, int agent_col, stdin_data *data) {
    if (step == 'U' && data->labirynth[agent_row - 1][agent_col] != '1' && data->labirynth[agent_row - 1][agent_col] != '3' &&
        data->labirynth[agent_row][agent_col] != '3') {
        return true;
    } else if (step == 'D' && data->labirynth[agent_row + 1][agent_col] != '1' && data->labirynth[agent_row + 1][agent_col] != '3' &&
               data->labirynth[agent_row][agent_col] != '3') {
        return true;
    } else if (step == 'L' && data->labirynth[agent_row][agent_col - 1] != '1' && data->labirynth[agent_row][agent_col - 1] != '2' &&
               data->labirynth[agent_row][agent_col] != '2') {
        return true;
    } else if (step == 'R' && data->labirynth[agent_row][agent_col + 1] != '1' && data->labirynth[agent_row][agent_col + 1] != '2' &&
               data->labirynth[agent_row][agent_col] != '2') {
        return true;
    } else {
        return false;
    }
}

void move(char step, int *agent_row, int *agent_col) {
    if (step == 'U') {
        *agent_row -= 1;
    } else if (step == 'D') {
        *agent_row += 1;
    } else if (step == 'L') {
        *agent_col -= 1;
    } else if (step == 'R') {
        *agent_col += 1;
    }
}

void traverse(genom *g, stdin_data *data) {
    char temp[g->size];
    int agent_row = data->agent_row;
    int agent_col = data->agent_col;
    int counter = 0;
    for (int i=0; i<g->size; i++) {
        if (check_move(g->path[i], agent_row, agent_col, data)) {
            move(g->path[i], &agent_row, &agent_col);
            temp[counter] = g->path[i];
            counter++;
        }
        if (check_in_exit(agent_row, agent_col, data)) {
            g->did_exit = true;
            g->size = counter;
            for (int i=0; i<counter; i++) {
                g->path[i] = temp[i];
            }
            return;
        }
    }

    g->did_exit = false;
    g->size = counter;
    for (int i=0; i<counter; i++) {
        g->path[i] = temp[i];
    }
}

int min(int a, int b) {
    if (a <= b) {
        return a;
    } else {
        return b;
    }
}

char reverse_step(char prev_step) {
    if (prev_step == 'U') {
        return 'D';
    } else if (prev_step == 'D') {
        return 'U';
    } else if (prev_step == 'R') {
        return 'L';
    } else if (prev_step == 'L') {
        return 'R';
    } else {
        return prev_step;
    }
}

bool random_moves(genom *g, stdin_data *data, int max_genom_size) {
    pcg32_random_t rng;
    pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&(rng));
    char moves[4] = {'U', 'D', 'L', 'R'};
    int rep_tres = 10;
    int agent_row;
    int agent_col;
    char prev_step;
    int counter;

RES:agent_row = data->agent_row;
    agent_col = data->agent_col;
    prev_step = ' ';
    counter = 0;
    while (counter < max_genom_size) {
        char step = moves[(int)pcg32_boundedrand_r(&rng, 4)];
        int rep_ctr = 0;
        while (!check_move(step, agent_row, agent_col, data) || step == reverse_step(prev_step)) {
            step = moves[(int)pcg32_boundedrand_r(&rng, 4)];
            if (rep_ctr < rep_tres) {
                rep_ctr++;
            } else {
                goto RES;
            }
        }

        prev_step = step;
        int str_size = (int)pcg32_boundedrand_r(&rng, min(data->col, data->row)) + 1;
        for (int i=0; i<str_size; i++) {
            if (counter >= max_genom_size) {
                break;
            } else if (check_move(step, agent_row, agent_col, data)) {
                move(step, &agent_row, &agent_col);
                g->path[counter] = step;
                counter++;
                if (check_in_exit(agent_row, agent_col, data)) {
                    g->size = counter;
                    g->did_exit = true;

                    return true;
                }
            } else {
                break;
            }
        }
    }

    g->size = counter;
    g->did_exit = false;

    return false;
}
