from math import cos
from math import pi
from math import sqrt
import time
from random import uniform
from math import e


def f(x1, x2, x3, x4):
    return round(1 - cos(2*pi*sqrt(x1**2 + x2**2 + x3**2 + x4**2)) + 0.1*sqrt(x1**2 + x2**2 + x3**2 + x4**2), 16)


def take_data():
    t, x1, x2, x3, x4 = [float(x) for x in input().split(" ")]
    return t, x1, x2, x3, x4


def apply_perturbations(x1_ac, x2_ac, x3_ac, x4_ac, step):
    k = uniform(-1, 1)
    return (
            x1_ac + k*step[0],
            x2_ac + k*step[1],
            x3_ac + k*step[2],
            x4_ac + k*step[3],
    )


def probability(d_f, T):
    return e**(-(d_f/T))


def random_vector(maximum):
    return [
            uniform(-maximum, maximum),
            uniform(-maximum, maximum),
            uniform(-maximum, maximum),
            uniform(-maximum, maximum),
    ]

def minimalize(t, x1, x2, x3, x4, T_0 = 1000.0, c = 0.5): # 4615 do utraty precyzji, przy c=0.5 to bedzie 1085, 3363 for 0.8, 2872 for 0.76
    x1_ac, x2_ac, x3_ac, x4_ac = x1, x2, x3, x4
    x1_best, x2_best, x3_best, x4_best = x1, x2, x3, x4
    f_best = f(x1_best, x2_best, x3_best, x4_best)
    f_acc = f_best

    T = T_0

    step = random_vector(2.0*max(abs(x1_best), abs(x2_best), abs(x3_best), abs(x4_best)))

    T_change_criteria = 5*1085
    T_change = 0

    max_T_change = 1085
    reset = 0

    step_change_criteria = 20
    step_change = 0

    t_start = time.process_time()
    while time.process_time() - t_start < t and T > 0:
        x1_temp, x2_temp, x3_temp, x4_temp = apply_perturbations(x1_ac, x2_ac, x3_ac, x4_ac, step)
        f_temp = f(x1_temp, x2_temp, x3_temp, x4_temp)

        if f_temp <= f_acc:
            f_acc = f_temp
            x1_ac, x2_ac, x3_ac, x4_ac = x1_temp, x2_temp, x3_temp, x4_temp
            if f_acc < f_best:
                x1_best, x2_best, x3_best, x4_best = x1_ac, x2_ac, x3_ac, x4_ac
                f_best = f_acc
        elif uniform(0, 1) <= probability(f_temp - f_acc, T):
            f_acc = f_temp
            x1_ac, x2_ac, x3_ac, x4_ac = x1_temp, x2_temp, x3_temp, x4_temp
        
        if T_change >= T_change_criteria:
            if (T*c > 0):
                T *= c
            T_change = 0
            reset += 1
        else:
            T_change += 1
        
        if reset >= max_T_change:
            x1_ac, x2_ac, x3_ac, x4_ac = x1_best, x2_best, x3_best, x4_best
            f_acc = f_best
            T = T_0
            reset = 0
            step_change = 0
            step = random_vector(1)
        
        if step_change >= step_change_criteria:
            step_change = 0
            step = random_vector(2.0*max(abs(x1_ac), abs(x2_ac), abs(x3_ac), abs(x4_ac)))
        else:
            step_change += 1
        
        if f_best == 0.0:
            break
        
    return x1_best, x2_best, x3_best, x4_best


if __name__ == "__main__":
    #t, x1, x2, x3, x4 = take_data()
    #print(t, x1, x2, x3, x4)
    for _ in range(100):
        x1, x2, x3, x4 = minimalize(4.0, uniform(-10, 10), uniform(-10, 10), uniform(-10, 10), uniform(-10, 10))
        if (f(x1, x2, x3, x4) != 0.0):
            print(f(x1, x2, x3, x4))
            print("Fail")
    #x1, x2, x3, x4 = minimalize(t, x1, x2, x3, x4)
    #print(x1, x2, x3, x4, f(x1, x2, x3, x4))
