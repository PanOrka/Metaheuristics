#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "pcg_basic.h"

#define high 1000000
#define fraction 0.000001
#define MIN -1000.0
#define MAX 1000.0

#define set_alfa 10
#define set_alfa2 1.0
#define alfa_multiplier 0.01
#define alfa_treshold 2
#define OPT 40

#define R 100
#define r 0.1
#define r_more 100
#define r_less 0.1
#define R_more 100
#define R_less 0.1

#define best_low 0.1
#define best_high 0.5

#define reset 10
#define max_stuck 5

#define r_min 0.0000001

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
    double sum = p.x1*p.x1 + p.x2*p.x2 + p.x3*p.x3 + p.x4*p.x4; // ||x||^2
    double mul = cos(p.x1 / 1.0)*cos(p.x2 / sqrt(2))*cos(p.x3 / sqrt(3))*cos(p.x4 / sqrt(4));

    return 1.0 + sum/4000.0 - mul;
}

void gradient_value(vector *p, double alfa) { // gradient f(x(t+1))
    double a = alfa*((sin((*p).x1)*cos((*p).x2)*cos((*p).x3)*cos((*p).x4))/(2.0*sqrt(6.0))+(*p).x1/2000.0);

    double b = alfa*((sin((*p).x2)*cos((*p).x1)*cos((*p).x3)*cos((*p).x4))/(2.0*sqrt(6.0))+(*p).x2/2000.0);

    double c = alfa*((sin((*p).x3)*cos((*p).x2)*cos((*p).x1)*cos((*p).x4))/(2.0*sqrt(6.0))+(*p).x3/2000.0);

    double d = alfa*((sin((*p).x4)*cos((*p).x2)*cos((*p).x3)*cos((*p).x1))/(2.0*sqrt(6.0))+(*p).x4/2000.0);

    (*p).x1 -= a;
    (*p).x2 -= b;
    (*p).x3 -= c;
    (*p).x4 -= d;
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
    
    if (grad1 == 0 && grad2 == 0) {
        return p1.value < p2.value;
    } else if (grad1 == 0) { // trafilismy na punkt siodlowy
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

    vec1_close.value = f(vec1_close);
    vec1_far.value = f(vec1_far);

    if (compare_gradients(vec1_close, vec1_far)) {
        *actual = vec1_close;
    } else {
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

        if (actual.value < the_best.value) { // na cele MULTI STARTU, bo ciezko tutaj przeszukiwac sensownie sasiedztwo
            the_best = actual;
        }

        if (actual.value < best.value) {
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
            reset_vector(&actual, the_best, best_low, best_high, rng);
            gradient_value(&actual, alfa);
            actual.value = f(actual);

            stuck = 0;
            res = 0;
            Rect = R;
            rect = r;
            alfa = set_alfa;
        } else if (stuck <= reset) {
            shift_vector(&actual, best, rect, Rect, rng);
        } else {
            shift_vector(&actual, actual, rect*r_more, Rect*R_more, rng); // dla proby robimy skok z aktualnego
            stuck = 0;
            Rect = R;
            rect = r;
        }
    }

    return best;
}

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