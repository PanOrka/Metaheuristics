#include "tsp.h"

int * find_best(town *towns, double max_sec, int towns_n) {
    int seed_int = 5;
    pcg32_random_t rng;
    pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&seed_int); // seeding rng (non deterministic)

    /*
    //
    // Alokujemy permutacje, wypelniamy wartosciami
    //
    */
    int *perm = malloc(sizeof(int)*(towns_n+1));
    for (int i = 0; i<towns_n; i++) {
        *(perm + i) = i+1;
    }
    *(perm+towns_n) = 1;

    /*
    //
    // Tworzymy Tabu i dobieramy parametry
    //
    */
    list *tabu = createList(); // malloc
    bool isOnTabu1, isOnTabu2;
    int max_tabu_size = towns_n; // ustalamy rozmiar tabu (maks) podczas 2 opt mamy n po 2 przestawien

    /*
    //
    // Ustalamy reset
    //
    */
    int reset = 5;
    int iter = reset;

    /*
    //
    // Alokujemy permutacje oraz odkladamy na stacku zmienne dla dlugosci drog
    //
    */
    int best_tour_len, actual_tour_len, temp_best_len, go_back;
    int *best_perm = malloc(sizeof(int)*(towns_n+1));

    /*
    //
    // Tworzymy tablice historii, na podstawie ktorej wyznaczamy karencje
    // aktualnie zapamietujemy ilosc krokow o wartosci ilosc_miast/2
    */
    history h = {malloc(sizeof(pair)*towns_n/2), 0, 0, towns_n/2}; // MALLOC NA ZAPAMIETYWANIE HISTORII K-KROKOW

    time_t t_start = clock(); // start czasu

    /*
    //
    // Losujemy permutacje i ustawiamy ja jako najlepsza
    //
    */
    random_permutation(perm, towns_n+1, &rng); // losowo permutujemy srodkowe miasta TUTAJ miejsce miasta w towns = perm - 1
    actual_tour_len = tour_len(towns, perm, towns_n+1);

    for (int i=0; i<towns_n+1; i++) {
        *(best_perm + i) = *(perm + i);
    }

    best_tour_len = actual_tour_len;

    /*
    // TUTAJ ZACZYNAMY ALGORYTM
    // 2-opt z uzyciem listy 2 kierunkowej
    //
    */
    while ((double)(clock() - t_start)/CLOCKS_PER_SEC < max_sec) {
        if (iter >= reset) {
            random_permutation(perm, towns_n+1, &rng); // losowo permutujemy srodkowe miasta TUTAJ miejsce miasta w towns = perm - 1
            actual_tour_len = tour_len(towns, perm, towns_n+1);
            temp_best_len = actual_tour_len;

            iter = 0;
        }

        /*
        //
        // 2-OPT
        //
        */
        for (int i=1; i<towns_n-1; i++) {
            for (int k=i+1; k<towns_n; k++) {
                isOnTabu1 = false;
                isOnTabu2 = false;
                /*
                // dodaje do tabu tak ze i < k
                // (value1 <- i, value2 <- k)
                // patrzymy na krawedz value1 -> value2
                */
                for (int t=0; t<(*tabu).size && t<max_tabu_size; t++) {
                    /*
                    // krawedzie (... i-1 i ... k k+1 ...) => (...i-1 k ... i k+1 ...)
                    // sprawdzamy czy sa takie krawedzie juz w tabu
                    //
                    */
                    if ((*getElement(tabu, t)).value1 == perm[i-1] && (*getElement(tabu, t)).value2 == perm[k]) {
                        isOnTabu1 = true;                   
                    }
                    if ((*getElement(tabu, t)).value1 == perm[i] && (*getElement(tabu, t)).value2 == perm[k+1]) {
                        isOnTabu2 = true;
                    }
                }

                /*
                //
                // Jesli nie ma przynajmniej jednej z krawedzi w tabu to probujemy wykonac 2-opt swap
                //
                */
                if (!isOnTabu1 || !isOnTabu2) {
                    go_back = actual_tour_len; // zapamietujemy zeby nie liczyc z nowu
                    opt2(perm, i, k);
                    actual_tour_len = tour_len(towns, perm, towns_n+1);

                    /*
                    //
                    // Jesli 2-opt da poprawe do switchujemy do tego rozwiazania
                    //
                    */
                    if (actual_tour_len <= temp_best_len) {
                        addToHistory(&h, perm[i-1], perm[i]); // Dodajemy do historii
                        addToHistory(&h, perm[k], perm[k+1]); // Dodajemy do historii

                        /* dodajemy do tabu krawedzie jezeli ich nie bylo
                        // karencje ustalamy na podstawie zmian w historii
                        // ustalam karencje jako ilosc wystapien w ilosci krokow podanych przy stworzeniu historii
                        // jesli nie bylo to ustalam niska karencje, np 1 (ewentualnie modyfikuje wspolczynniki)
                        //
                        // dodawanie z dana karencja (w tabu, rozwiazania beda sie kolejkowaly z okreslona karencja (zalezna od historii ostatniego wyboru))
                        */
                        if (!isOnTabu1) { // po wykonaniu ruchu dodajemy go do tabu
                            addElement(tabu, perm[i-1], perm[i], check_in_history(&h, perm[i-1], perm[i]));
                        }
                        if (!isOnTabu2) {
                            addElement(tabu, perm[k], perm[k+1], check_in_history(&h, perm[k], perm[k+1]));
                        }
                        
                        temp_best_len = actual_tour_len;
                    } else {
                        opt2(perm, i, k); // brak poprawy, wracamy, ponownie zamieniamy i oraz k w permutacji i cofamy sie do starej dlugosci trasy
                        actual_tour_len = go_back;
                    }
                }
            }
        }

        /*
        //
        // Czyszczenie tabu
        //
        */
        clearTabu(tabu, max_tabu_size);

        /*
        //
        // Jesli znalezlismy lepszy niz najlepszy to przepisujemy permutacje do best_perm
        //
        */
        if (temp_best_len < best_tour_len) {
            for (int i=1; i<towns_n; i++) {
                *(best_perm + i) = *(perm + i);
            }

            best_tour_len = temp_best_len;
            iter = 0; // dostalismy poprawe wiec zerujemy
        } else {
            iter++; // tutaj nie dostalismy poprawy wiec zwiekszamy iteracje
        }
    }

    /*
    //
    // zwalniamy pamiec i zwracamy najlepsza permutacje
    //
    */
    free(perm);
    deleteList(tabu);
    free(tabu);
    free(h.elements);

    return best_perm;
}

int main() {
    double max_sec;
    int towns_n;

    scanf("%lf", &max_sec);
    scanf("%d", &towns_n);

    town *towns = malloc(sizeof(town)*towns_n);

    for (int i=0; i<towns_n; i++) {
        towns[i].d = malloc(sizeof(int)*towns_n); // alokujemy na towns_n intow
        for (int k=0; k<towns_n; k++) {
            scanf("%d", towns[i].d + k);
        }
    }

    int *best = find_best(towns, max_sec, towns_n);

    printf("%d\n", tour_len(towns, best, towns_n+1));
    for (int i = 0; i<towns_n+1; i++) {
        printf("%d ", *(best+i));
    }
    printf("\n");

    return 0;
}