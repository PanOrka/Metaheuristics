import numpy
from random import choice
import copy
import time
from random import uniform

from block_creator import block, create_blocks_LD, create_blocks_LU, create_blocks_RD, create_blocks_RU
from matrix_functionality import cost_func, create_from_blocks, search_neighbourhood, probability


def aprox_matrix(M, k, n, m, t, T_0 = 1000.0, c=0.5):
    aprox_M = [[0 for _ in range(m)] for _ in range(n)]

    create_blocks = [create_blocks_RD, create_blocks_RU, create_blocks_LU, create_blocks_LD]

    blocks, special_blocks_R, special_blocks_D = choice(create_blocks)(n, m, k)

    t_start = time.process_time()
    
    for x in blocks:
        for b in x:
            b.calc_avg(M)
    
    create_from_blocks(aprox_M, blocks, n, m)

    # INIT
    T = T_0

    best_solve = (blocks, special_blocks_R, special_blocks_D)
    best_value = cost_func(M, aprox_M, n, m)
    current_solve = (copy.deepcopy(blocks), copy.deepcopy(special_blocks_R), copy.deepcopy(special_blocks_D))
    current_value = best_value

    # RESTART CRITERIA
    T_change = 0
    T_change_criteria = max(n, m)

    max_T_change = 100
    reset = 0

    same_value_counter = 0
    same_value_counter_treshold = max(n, m)
    
    while time.process_time() - t_start < t and T > 0:
        # przeszukujemy sasiedztwo rozwiazania, probkujac
        temp_solve, is_same = search_neighbourhood((copy.deepcopy(current_solve[0]), copy.deepcopy(current_solve[1]), copy.deepcopy(current_solve[2])))

        if not is_same:
            for x in temp_solve[0]:
                for b in x:
                    b.calc_avg(M)
            
            create_from_blocks(aprox_M, temp_solve[0], n, m)

            temp_value = cost_func(M, aprox_M, n, m)

            if temp_value == current_value:
                same_value_counter += 1
            else:
                same_value_counter = 0

            if temp_value <= current_value:
                current_solve = ((copy.deepcopy(temp_solve[0]), copy.deepcopy(temp_solve[1]), copy.deepcopy(temp_solve[2])))
                current_value = temp_value

                if current_value < best_value:
                    best_solve = (copy.deepcopy(current_solve[0]), copy.deepcopy(current_solve[1]), copy.deepcopy(current_solve[2]))
                    best_value = current_value
            elif uniform(0, 1) <= probability(temp_value - current_value, T):
                current_solve = ((copy.deepcopy(temp_solve[0]), copy.deepcopy(temp_solve[1]), copy.deepcopy(temp_solve[2])))
                current_value = temp_value
        else:
            same_value_counter += 1

        if T_change >= T_change_criteria:
            T *= c
            T_change = 0
            reset += 1
        else:
            T_change += 1
        
        if same_value_counter >= same_value_counter_treshold:
            reset = max_T_change
        
        if reset >= max_T_change:
            T = T_0
            current_solve = choice(create_blocks)(n, m, k) # FULL_RESTART
            for x in current_solve[0]:
                for b in x:
                    b.calc_avg(M)
            create_from_blocks(aprox_M, current_solve[0], n, m)
            current_value = cost_func(M, aprox_M, n, m)
            reset = 0
            same_value_counter = 0

    create_from_blocks(aprox_M, best_solve[0], n, m)
    return best_value, aprox_M


def get_first_line():
    t, n, m, k = [float(x) for x in input().split(" ")]
    return t, n, m, k


def get_matrix(n):
    return [[int(x) for x in [k for k in input().split(" ") if k != '']] for _ in range(n)]


if __name__ == "__main__":
    t, n, m, k = get_first_line()
    M = get_matrix(int(n))
    best_value, aprox_M = aprox_matrix(M, int(k), int(n), int(m), t)
    print("\nBEST VALUE WAS:", best_value)
    print("Matrix:")
    print(numpy.matrix(aprox_M))