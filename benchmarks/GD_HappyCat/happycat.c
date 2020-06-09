#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "pcg_basic.h"

#define high 1000000
#define fraction 0.000001
#define MIN -100.0
#define MAX 100.0

#define set_alfa 1.0
#define set_alfa2 0.1
#define alfa_multiplier 0.001
#define alfa_treshold 5
#define OPT 30

#define R 0.1
#define r 0.001
#define r_more 10
#define r_less 0.1
#define R_more 10
#define R_less 0.1

#define best_low 0.01
#define best_high 0.02

#define reset 40
#define max_stuck 10

#define r_min 0.0000000000001

typedef struct {
    double x1, x2, x3, x4, value;
} vector;

vector random_vector(double minimum, double maximum, pcg32_random_t *rng) {
    vector rand_vec = {
        minimum + (((double)pcg32_boundedrand_r(rng, high))*fraction)*(maximum - minimum),
        minimum + (((double)pcg32_boundedrand_r(rng, high))*fraction)*(maximum - minimum),
        minimum + (((double)pcg32_boundedrand_r(rng, high))*fraction)*(maximum - minimum),
        minimum + (((double)pcg32_boundedrand_r(rng, high))*fraction)*(maximum - minimum),
        0
        };

    return rand_vec;
}

double f(vector p) { // f(x)
    double vec_len2 = p.x1*p.x1 + p.x2*p.x2 + p.x3*p.x3 + p.x4*p.x4; // ||x||^2
    double sum = p.x1 + p.x2 + p.x3 + p.x4;

    double el = vec_len2 - 4.0;
    double left_el = pow(el*el, 0.125); // ^1/8

    return 0.5 + left_el + 0.125*vec_len2 + 0.25*sum;
}

void gradient_value(vector *p, double alfa) { // gradient f(x(t+1))
    double sum_sqrts = (*p).x1*(*p).x1 + (*p).x2*(*p).x2 + (*p).x3*(*p).x3 + (*p).x4*(*p).x4 - 4.0;
    if (sum_sqrts > 0) { // tutaj nieciagla pochodna moze sie zdarzyc, wtedy nic nie robimy
        double down_el = sum_sqrts*sum_sqrts; // ^2 pierw to zeby wynik byl dodatni
        down_el = pow(down_el, 0.875); // ^7/8 a potem pierwiastek, inaczej sie psuje
        double whole = (sum_sqrts/down_el);

        (*p).x1 = (*p).x1 - alfa*((0.25)*((2.0*(*p).x1*whole) + (*p).x1 + 1.0));
        (*p).x2 = (*p).x2 - alfa*((0.25)*((2.0*(*p).x2*whole) + (*p).x2 + 1.0));
        (*p).x3 = (*p).x3 - alfa*((0.25)*((2.0*(*p).x3*whole) + (*p).x3 + 1.0));
        (*p).x4 = (*p).x4 - alfa*((0.25)*((2.0*(*p).x4*whole) + (*p).x4 + 1.0));
    }
}

bool compare(vector p1, vector p2) {
    return p1.x1 == p2.x1 && p1.x2 == p2.x2 && p1.x3 == p2.x3 && p1.x4 == p2.x4;
}

bool compare_gradients(vector p1, vector p2) {
    double grad1, grad2;
    double sum_sqrts = p1.x1*p1.x1 + p1.x2*p1.x2 + p1.x3*p1.x3 + p1.x4*p1.x4 - 4.0;
    if (sum_sqrts > 0) {
        double down_el = sum_sqrts*sum_sqrts;
        down_el = pow(down_el, 0.875); // ^7/8 a potem pierwiastek, inaczej sie psuje
        double whole = (sum_sqrts/down_el);

        grad1 += abs((0.25)*((2.0*p1.x1*whole) + p1.x1 + 1.0));
        grad1 += abs((0.25)*((2.0*p1.x2*whole) + p1.x2 + 1.0));
        grad1 += abs((0.25)*((2.0*p1.x3*whole) + p1.x3 + 1.0));
        grad1 += abs((0.25)*((2.0*p1.x4*whole) + p1.x4 + 1.0));
    } else {
        grad1 = 0;
    }

    sum_sqrts = p2.x1*p2.x1 + p2.x2*p2.x2 + p2.x3*p2.x3 + p2.x4*p2.x4 - 4.0;
    if (sum_sqrts > 0) {
        double down_el = sum_sqrts*sum_sqrts;
        down_el = pow(down_el, 0.875); // ^7/8 a potem pierwiastek, inaczej sie psuje
        double whole = (sum_sqrts/down_el);

        grad2 += abs((0.25)*((2.0*p2.x1*whole) + p2.x1 + 1.0));
        grad2 += abs((0.25)*((2.0*p2.x2*whole) + p2.x2 + 1.0));
        grad2 += abs((0.25)*((2.0*p2.x3*whole) + p2.x3 + 1.0));
        grad2 += abs((0.25)*((2.0*p2.x4*whole) + p2.x4 + 1.0));
    } else {
        grad2 = 0;
    }

    if (grad1 == 0) {
        return true;
    } else if (grad2 == 0) {
        return false;
    } else {
        return grad1 > grad2;
    }
}

void shift_vector(vector *actual, vector best, double rect, double Rect, pcg32_random_t *rng) {
    vector vec1_close = {
        best.x1 - rect + ((double)pcg32_boundedrand_r(rng, high))*fraction*2*rect,
        best.x2 - rect + ((double)pcg32_boundedrand_r(rng, high))*fraction*2*rect,
        best.x3 - rect + ((double)pcg32_boundedrand_r(rng, high))*fraction*2*rect,
        best.x4 - rect + ((double)pcg32_boundedrand_r(rng, high))*fraction*2*rect,
    };

    vector vec1_far;

    if (pcg32_boundedrand_r(rng, 2)) {
        vec1_far.x1 = best.x1 + rect + ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    } else {
        vec1_far.x1 = best.x1 - rect - ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    }

    if (pcg32_boundedrand_r(rng, 2)) {
        vec1_far.x2 = best.x2 + rect + ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    } else {
        vec1_far.x2 = best.x2 - rect - ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    }

    if (pcg32_boundedrand_r(rng, 2)) {
        vec1_far.x3 = best.x3 + rect + ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    } else {
        vec1_far.x3 = best.x3 - rect - ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    }

    if (pcg32_boundedrand_r(rng, 2)) {
        vec1_far.x4 = best.x4 + rect + ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    } else {
        vec1_far.x4 = best.x4 - rect - ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    }

    if (compare_gradients(vec1_close, vec1_far)) {
        vec1_close.value = f(vec1_close);
        *actual = vec1_close;
    } else {
        vec1_far.value = f(vec1_far);
        *actual = vec1_far;
    }
}

void reset_vector(vector *actual, vector best, double rect, double Rect, pcg32_random_t *rng) {
    vector vec1_far;

    if (pcg32_boundedrand_r(rng, 2)) {
        vec1_far.x1 = best.x1 + rect + ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    } else {
        vec1_far.x1 = best.x1 - rect - ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    }

    if (pcg32_boundedrand_r(rng, 2)) {
        vec1_far.x2 = best.x2 + rect + ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    } else {
        vec1_far.x2 = best.x2 - rect - ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    }

    if (pcg32_boundedrand_r(rng, 2)) {
        vec1_far.x3 = best.x3 + rect + ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    } else {
        vec1_far.x3 = best.x3 - rect - ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    }

    if (pcg32_boundedrand_r(rng, 2)) {
        vec1_far.x4 = best.x4 + rect + ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    } else {
        vec1_far.x4 = best.x4 - rect - ((double)pcg32_boundedrand_r(rng, high))*fraction*Rect;
    }

    vec1_far.value = f(vec1_far);
    *actual = vec1_far;
}

vector find_optimum(time_t t_start, double max_sec, pcg32_random_t *rng) {
    double alfa = set_alfa;
    int alfa_dec_treshold, opt_treshold;
    double Rect = R;
    double rect = r;
    int stuck = 0;
    int res = 0;

    vector actual, best, the_best;
    actual = random_vector(MIN, MAX, rng);

    gradient_value(&actual, alfa);
    actual.value = f(actual);
    best = actual;
    the_best = actual;

    while ((double)(clock() - t_start)/CLOCKS_PER_SEC < max_sec) {
        double temp_min = actual.value;

        opt_treshold = 0;
        for (alfa_dec_treshold=0; alfa_dec_treshold < alfa_treshold; alfa_dec_treshold++) {
            for (opt_treshold; opt_treshold < OPT; opt_treshold++) {
                gradient_value(&actual, alfa);
                actual.value = f(actual);
                if (actual.value < temp_min) {
                    temp_min = actual.value;
                } else {
                    break;
                }
            }

            alfa *= alfa_multiplier;
        }

        alfa = set_alfa2;

        printf("Dostalismy vector: %f, %f, %f, %f\nPrzy wartosci f(vector): %f\nAlgorytmowi zajelo to: %f sekund\n",
        actual.x1, actual.x2, actual.x3, actual.x4, actual.value, (double)(clock() - t_start)/CLOCKS_PER_SEC);
        printf("Rect: %f, rect: %.15f\n", Rect, rect);

        if (actual.value < the_best.value) { // na cele MULTI STARTU, bo ciezko tutaj przeszukiwac sensownie sasiedztwo
            the_best = actual;
        }

        if (actual.value < best.value) { // tutaj moze byc <=
            best = actual;
            Rect = R;
            rect = r;
            res = 0;
            stuck = 0;
        } else {
            stuck++;
            res++;
        }

        if (stuck >= max_stuck && rect > r_min) {
            Rect *= R_less;
            rect *= r_less;
        }

        if (res >= reset) {
            reset_vector(&actual, the_best, best_low, best_high, rng); // resetujemy w okolice najlepszego
            gradient_value(&actual, alfa);
            actual.value = f(actual);

            best = actual;

            stuck = 0;
            res = 0;
            Rect = R;
            rect = r;
            alfa = set_alfa;
        } else {
            if (stuck <= reset) {
                shift_vector(&actual, best, rect, Rect, rng);
            } else {
                shift_vector(&actual, best, rect, Rect, rng); // actual daje wieksze skoki, best daje blizsze minimum lok.
                stuck = 0;
                Rect = R;
                rect = r;
            }
        }
    }

    return the_best;
}
// UWAGA ZROBIC EWENTUALNIE MAKSYMALNY reset_vector, moze zeby losowalo z full przedzialu (chociaz nie wiem czy to dobre)
int main(int argc, char *argv[]) {
    int seed_int = 5;
    pcg32_random_t rng;
    pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&seed_int); // seeding rng (non deterministic)

    double max_sec = strtod(argv[1], NULL);
    vector best;
    time_t t_start = clock();
    best = find_optimum(t_start, max_sec, &rng);

    printf("Dostalismy vector: %f, %f, %f, %f\nPrzy wartosci f(vector): %f\nAlgorytmowi zajelo to: %f sekund\n",
    best.x1, best.x2, best.x3, best.x4, f(best), (double)(clock() - t_start)/CLOCKS_PER_SEC);
}