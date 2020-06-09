import time
from random import randint
from random import choice
from random import uniform
from data_gatherer import get_data
from agent_movement import check_in_exit, move, traverse, random_moves
from math import e


def probability(d_f, T):
    return e**(-(d_f/T))


def cost_func(path):
    return len(path)


def swap_2(path):
    k = randint(0, len(path)-1)
    j = randint(0, len(path)-1)
    temp = path[k]
    path[k] = path[j]
    path[j] = temp


def swap_3(path):
    k = randint(0, len(path)-1)
    j = randint(0, len(path)-1)
    l = randint(0, len(path)-1)
    temp = path[k]
    path[k] = path[j]
    path[j] = path[l]
    path[l] = temp


def swap2_4(path):
    k = randint(0, len(path)-2)
    j = randint(0, len(path)-2)
    temp = path[j], path[j+1]
    path[j] = path[k]
    path[j+1] = path[k+1]
    path[k] = temp[0]
    path[k+1] = temp[1]


def cut_2(path):
    k = randint(0, len(path)-2)
    del path[k]
    del path[k]


def insert_random_3(path):
    moves = ['U', 'L', 'D', 'R']
    i = randint(0, len(path))
    path.insert(i, choice(moves))
    i = randint(0, len(path))
    path.insert(i, choice(moves))
    i = randint(0, len(path))
    path.insert(i, choice(moves))


def insert_random_path_len(path):
    moves = ['U', 'L', 'D', 'R']
    for _ in range(len(path)):
        i = randint(0, len(path))
        path.insert(i, choice(moves))


def generate_neighbour(cur_path):
    temp_path = cur_path.copy()

    search_space = randint(0, 5)
    if search_space == 0 and len(temp_path) >= 2:
        swap_2(temp_path)
    elif search_space == 1 and len(temp_path) >= 3:
        swap_3(temp_path)
    elif search_space == 2 and len(temp_path) > 4:
        swap2_4(temp_path)
    elif search_space == 3 and len(temp_path) > 4:
        cut_2(temp_path)
    elif search_space == 4:
        insert_random_3(temp_path)
    elif search_space == 5:
        insert_random_path_len(temp_path)

    return temp_path


def find_way(agent_pos, crate, n, m, max_sec, T_0=1000.0, c=0.85):
    best_path = []

    # INIT
    T = T_0

    # run criteria
    n_search = 0

    t_start = time.process_time()
    while time.process_time() - t_start < max_sec:
        cur_pos = agent_pos.copy()
        cur_path = []
        while (not check_in_exit(cur_pos, crate)) and time.process_time() - t_start < max_sec:
            cur_pos = agent_pos.copy()
            cur_path = random_moves(cur_pos, crate, n, m, (n-2)*(m-2)) # agent zna rozmiar pola
        
        if time.process_time() - t_start >= max_sec:
            return best_path

        if len(best_path) == 0 or len(cur_path) < len(best_path):
            best_path = cur_path.copy()
        
        n_search_treshold = min(n, m)*cost_func(cur_path)

        while n_search < n_search_treshold and T > 0 and time.process_time() - t_start < max_sec:
            temp_path = generate_neighbour(cur_path)
            temp_path, did_exit = traverse(agent_pos.copy(), temp_path, crate)

            if did_exit:
                if cost_func(temp_path) <= cost_func(cur_path):
                    cur_path = temp_path.copy()
                    if cost_func(cur_path) < cost_func(best_path):
                        best_path = cur_path.copy()
                elif uniform(0, 1) <= probability(cost_func(temp_path) - cost_func(cur_path), T):
                    cur_path = temp_path.copy()
                T *= c
            else:
                n_search += 1
        
        T = T_0
        n_search = 0
        
    return best_path


if __name__ == "__main__":
    max_sec, n, m, crate, agent_pos = get_data()

    best_path = find_way(agent_pos, crate, n, m, max_sec)

    for l in best_path:
        move(l, agent_pos)
        crate[agent_pos[0]][agent_pos[1]] = '█'
    for i in crate:
        print([" " if k == '0' else k for k in i])
    print(len(best_path))
    print(best_path)
