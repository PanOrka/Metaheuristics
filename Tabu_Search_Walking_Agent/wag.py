import time
from random import randint


def check_in_exit(cur_pos, crate):
    return crate[cur_pos[0]][cur_pos[1]] == '8'


def check_cycle(step, prev_step):
    if step == 'U' and prev_step == 'D':
        return False
    elif step == 'D' and prev_step == 'U':
        return False
    elif step == 'L' and prev_step == 'R':
        return False
    elif step == 'R' and prev_step == 'L':
        return False
    else:
        return True


def check_move(step, cur_pos, crate):
    if step == 'U' and crate[cur_pos[0]-1][cur_pos[1]] != '1':
        return True
    elif step == 'D' and crate[cur_pos[0]+1][cur_pos[1]] != '1':
        return True
    elif step == 'L' and crate[cur_pos[0]][cur_pos[1]-1] != '1':
        return True
    elif step == 'R' and crate[cur_pos[0]][cur_pos[1]+1] != '1':
        return True
    else:
        return False


def move(step, cur_pos):
    if step == 'U':
        cur_pos[0] += -1
    elif step == 'D':
        cur_pos[0] += 1
    elif step == 'L':
        cur_pos[1] += -1
    elif step == 'R':
        cur_pos[1] += 1


def random_moves(cur_pos, crate, n, m, max_steps):
    path = []
    moves = ['U', 'D', 'L', 'R']
    step = ''
    prev_step = ''

    while len(path) <= max_steps:
        step = moves[randint(0, 3)]
        while not check_cycle(step, prev_step) or not check_move(step, cur_pos, crate):
            step = moves[randint(0, 3)]
        
        prev_step = step
        for _ in range(randint(1, min([n, m]))): # losujemy dlugosc ciagu ktory chcemy dodac
            if not check_move(step, cur_pos, crate):
                break

            move(step, cur_pos)
            path += [step]
            if check_in_exit(cur_pos, crate):
                return path

    return path


def traverse(cur_pos, path, crate):
    acc_path = []
    for step in path:
        if check_move(step, cur_pos, crate):
            move(step, cur_pos)
            acc_path += [step]
        if check_in_exit(cur_pos, crate):
            return acc_path, True
    return acc_path, False # petla minela bez znalezienia wyjscia


def find_way(agent_pos, crate, n, m, max_sec):
    best_path = []
    cur_path = []
    cur_pos = []
    tabu = []

    t_start = time.process_time()
    while time.process_time() - t_start < max_sec:
        cur_pos = agent_pos.copy()
        cur_path = []
        while (not check_in_exit(cur_pos, crate)) and time.process_time() - t_start < max_sec:
            cur_pos = agent_pos.copy()
            cur_path = random_moves(cur_pos, crate, n, m, (n-2)+(m-2)) # agent zna rozmiary pola wiec ustalam maksymalna liczbe krokow na przejscie przez przekatna

        if len(best_path) == 0 or len(cur_path) < len(best_path):
            best_path = cur_path.copy()

        tabu += [cur_path.copy()] # wrzucamy sobie aktualne minimum do tabu
        temp_path = cur_path.copy()
        # tutaj nie tylko chcemy powstrzymac sie od unikania tych samych minimow lokalnych, ale rowniez na unikniecie niepotrzebnego spacerowania
        # po drogach ktore daja wynik niedopuszczalny, ustalamy male sasiedztwo, ktore probkujemy oraz ogromna tablice tabu
        
        for _ in range(min([n, m, len(best_path)])): # ustalamy nasze sasiedztwo na podstawie n, m lub dlugosci najlepszej drogi
            k = randint(0, len(temp_path)-2)
            j = randint(k+1, len(temp_path)-1) # probkujemy sasiedztwo
            temp = temp_path[k]
            temp_path[k] = temp_path[j]
            temp_path[j] = temp
            if not temp in tabu:
                path_after_swap, did_exit = traverse(agent_pos.copy(), temp_path, crate)
                if did_exit and len(path_after_swap) < len(best_path):
                    best_path = path_after_swap.copy()
                    tabu += [path_after_swap.copy()]
                elif len(temp_path) != len(path_after_swap):
                    tabu += [temp_path.copy(), path_after_swap.copy()]
                else:
                    tabu += [temp_path.copy()]
        while len(tabu) >= n*m:
            del tabu[0]
        
    return best_path


def get_data():
    frst_line = str(input())
    lst = frst_line.split(" ")
    max_sec = float(lst[0])
    n = int(lst[1])
    m = int(lst[2])
    agent_pos = []
    crate = []
    for i in range (n):
        line = str(input())
        if '5' in line:
            agent_pos += [i] # wiersz
            agent_pos += [line.find('5')] # kolumna => agent = [wiersz, kolumna]
        temp = []
        for letter in line:
            if letter != '\n':
                temp += [letter]
        crate += [temp]

    return (max_sec, n, m, crate, agent_pos)


if __name__ == "__main__":
    max_sec, n, m, crate, agent_pos = get_data()
    
    best_path = find_way(agent_pos, crate, n, m, max_sec)

    for l in best_path:
        move(l, agent_pos)
        crate[agent_pos[0]][agent_pos[1]] = '█'
    for i in crate:
        print(i)
    print(len(best_path))
    print(best_path)
