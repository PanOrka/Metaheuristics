#include "tsp.h"

void random_permutation(int *tab, int size, pcg32_random_t *rng) {
    int r1, r2, temp;

    for (int i=0; i<size/2; i++) { // tutaj mieszamy sobie losowo elementy permutacji
        r1 = (int)pcg32_boundedrand_r(rng, size-2) + 1;
        r2 = (int)pcg32_boundedrand_r(rng, size-2) + 1;
        temp = *(tab + r1);
        *(tab + r1) = *(tab + r2);
        *(tab + r2) = temp;
    }
}

int tour_len(town *towns, int *tab, int size) { // tab_size
    int sum = 0;
    for (int i=1; i<size; i++) {
        sum += *((*(towns + *(tab + i - 1) - 1)).d + *(tab + i) - 1);
    }

    return sum;
}

void addToHistory(history *h, int i, int k) {
    pair temp = {i, k};
    *((*h).elements + (*h).start_ptr) = temp;

    if ((*h).start_ptr == (*h).size-1) { // start_ptr jest ostatni teraz
        (*h).start_ptr = 0;
    } else {
        (*h).start_ptr++;
    }

    if ((*h).end_ptr < (*h).size) { // maksymalny end_ptr
        (*h).end_ptr++;
    }
}

int check_in_history(history *h, int i, int j) {
    int occurencies = 0;
    for (int k=0; k<(*h).end_ptr; k++) {
        if ((*((*h).elements + k)).i == i && (*((*h).elements + k)).j == j) {
            occurencies++;
        }
    }

    return occurencies+1;
}

void opt2(int *tab, int i, int k) {
    int swap = tab[i];
    tab[i] = tab[k];
    tab[k] = swap;
}

void clearTabu(list *tabu, int max_tabu_size) {
    for (int i=0; i<(*tabu).size && i<max_tabu_size; i++) {
        node *temp_node = getElement(tabu, i);
        (*temp_node).value3--; // zmniejszam ilosc karencji w tablicy tabu
        if ((*temp_node).value3 <= 0) {
            deleteElement(tabu, i);
        }
    }
}