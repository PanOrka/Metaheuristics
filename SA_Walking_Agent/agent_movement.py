from random import randint


def check_in_exit(cur_pos, crate):
    return crate[cur_pos[0]][cur_pos[1]] == '8' or (
        crate[cur_pos[0]][cur_pos[1] + 1] == '8' or
        crate[cur_pos[0] + 1][cur_pos[1]] == '8' or
        crate[cur_pos[0]][cur_pos[1] - 1] == '8' or
        crate[cur_pos[0] - 1][cur_pos[1]] == '8'
    )


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


def traverse(cur_pos, path, crate):
    acc_path = []
    for step in path:
        if check_move(step, cur_pos, crate):
            move(step, cur_pos)
            acc_path += [step]
        if check_in_exit(cur_pos, crate):
            return acc_path, True
    return acc_path, False # petla minela bez znalezienia wyjscia


def random_moves(cur_pos, crate, n, m, max_steps):
    path = []
    moves = ['U', 'D', 'L', 'R']
    step = ''

    while len(path) <= max_steps:
        step = moves[randint(0, 3)]
        while not check_move(step, cur_pos, crate):
            step = moves[randint(0, 3)]
        
        for _ in range(randint(1, min(n, m))): # losujemy dlugosc ciagu ktory chcemy dodac
            if not check_move(step, cur_pos, crate):
                break

            move(step, cur_pos)
            path += [step]
            if check_in_exit(cur_pos, crate):
                return path

    return path