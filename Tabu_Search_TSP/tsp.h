#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#include "pcg_basic.h"
#include "twlist.h"

/*
// struktura miast, mamy int *d ktory wskazuje na tablice odleglosci do kolejnych miast
*/
typedef struct {
    int *d;
} town;

/*
// struktura pary, potrzebna do zapamietywania historii skierowanych krawedzi
*/
typedef struct {
    int i, j;
} pair;

/*
// struktura historii, przechowujaca tablice par
*/
typedef struct {
    pair *elements;
    int start_ptr, end_ptr;
    int size;
} history;

/*
// tworzenie losowej permutacji
*/
void random_permutation(int *tab, int size, pcg32_random_t *rng);

/*
// obliczanie drogi w danej permutacji
*/
int tour_len(town *towns, int *tab, int size);

/*
// dodawanie krawedzi do historii
*/
void addToHistory(history *h, int i, int k);

/*
// przeszukiwanie historii ile wystapila dana krawedz skierowana w tablicy historii i zwrocenie ilosc+1
*/
int check_in_history(history *h, int i, int j);

/*
// 2-opt swap
*/
void opt2(int *tab, int i, int k);

/*
// aktualizacja karencji i czyszczenie tabu z elementow o karencji <= 0
*/
void clearTabu(list *tabu, int max_tabu_size);