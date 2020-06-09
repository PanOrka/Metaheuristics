from random import randint
from random import choice
from block_creator import block
from math import e


def cost_func(M, aprox_M, n, m):
    dmse = 0
    for i in range(n):
        for j in range(m):
            dmse += (M[i][j] - aprox_M[i][j])**2
    
    dmse *= 1/(n*m)
    return dmse


def create_from_blocks(aprox_M, blocks, n, m):
    for x in blocks:
        for b in x:
            for i in range(b.down_cord[0] - b.up_cord[0] + 1):
                for j in range(b.down_cord[1] - b.up_cord[1] + 1):
                    aprox_M[j+b.up_cord[1]][i+b.up_cord[0]] = b.avg


def search_neighbourhood(current_solve):
    is_same = True
    if randint(0, 1) == 0: # wybieramy ruch jednej ze sciany
        indexes = choice(current_solve[1]) # PRAWO [y, x]
        d_move = randint(-indexes[1], len(current_solve[0][indexes[0]]) - 1 - indexes[1])
        b_to_move = current_solve[0][indexes[0]][indexes[1]]
        if (d_move > 0):
            for i in range(1, d_move+1):
                b_to_check = current_solve[0][indexes[0]][indexes[1] + i]
                if b_to_check.up_cord[1] != b_to_move.up_cord[1] or b_to_check.down_cord[1] != b_to_move.down_cord[1]: # Jesli mamy juz shift na y w danej kolumnie to nie robimy nic
                    break
            else:
                for i in range(1, d_move+1):
                    b_to_move.up_cord[0] += (b_to_check.down_cord[0] - b_to_check.up_cord[0] + 1) # przesuwamy o k
                    b_to_move.down_cord[0] += (b_to_check.down_cord[0] - b_to_check.up_cord[0] + 1) # przesuwamy o k

                    b_to_check = current_solve[0][indexes[0]][indexes[1] + i]
                    b_to_check.up_cord[0] -= (b_to_move.down_cord[0] - b_to_move.up_cord[0] + 1)
                    b_to_check.down_cord[0] -= (b_to_move.down_cord[0] - b_to_move.up_cord[0] + 1)
                    current_solve[0][indexes[0]][indexes[1] + i] = b_to_move
                    current_solve[0][indexes[0]][indexes[1] + i-1] = b_to_check
                indexes[1] += d_move
                is_same = False
        elif (d_move < 0):
            for i in range(-1, d_move-1, -1):
                b_to_check = current_solve[0][indexes[0]][indexes[1] + i]
                if b_to_check.up_cord[1] != b_to_move.up_cord[1] or b_to_check.down_cord[1] != b_to_move.down_cord[1]: # Jesli mamy juz shift na y w danej kolumnie to nie robimy nic
                    break
            else:
                for i in range(-1, d_move-1, -1):
                    b_to_move.up_cord[0] -= (b_to_check.down_cord[0] - b_to_check.up_cord[0] + 1) # przesuwamy o k
                    b_to_move.down_cord[0] -= (b_to_check.down_cord[0] - b_to_check.up_cord[0] + 1) # przesuwamy o k

                    b_to_check = current_solve[0][indexes[0]][indexes[1] + i]
                    b_to_check.up_cord[0] += (b_to_move.down_cord[0] - b_to_move.up_cord[0] + 1)
                    b_to_check.down_cord[0] += (b_to_move.down_cord[0] - b_to_move.up_cord[0] + 1)
                    current_solve[0][indexes[0]][indexes[1] + i] = b_to_move
                    current_solve[0][indexes[0]][indexes[1] + i+1] = b_to_check
                indexes[1] += d_move
                is_same = False
    else:
        indexes = choice(current_solve[2]) # DOL [y, x]
        d_move = randint(-indexes[0], len(current_solve[0]) - 1 - indexes[0])
        b_to_move = current_solve[0][indexes[0]][indexes[1]]
        if (d_move > 0):
            for i in range(1, d_move+1):
                b_to_check = current_solve[0][indexes[0] + i][indexes[1]]
                if b_to_check.up_cord[0] != b_to_move.up_cord[0] or b_to_check.down_cord[0] != b_to_move.down_cord[0]: # Jesli mamy juz shift na y w danej kolumnie to nie robimy nic
                    break
            else:
                for i in range(1, d_move+1):
                    b_to_move.up_cord[1] += (b_to_check.down_cord[1] - b_to_check.up_cord[1] + 1) # przesuwamy o k
                    b_to_move.down_cord[1] += (b_to_check.down_cord[1] - b_to_check.up_cord[1] + 1) # przesuwamy o k

                    b_to_check = current_solve[0][indexes[0] + i][indexes[1]]
                    b_to_check.up_cord[1] -= (b_to_move.down_cord[1] - b_to_move.up_cord[1] + 1)
                    b_to_check.down_cord[1] -= (b_to_move.down_cord[1] - b_to_move.up_cord[1] + 1)
                    current_solve[0][indexes[0] + i][indexes[1]] = b_to_move
                    current_solve[0][indexes[0] + i-1][indexes[1]] = b_to_check
                indexes[0] += d_move
                is_same = False
        elif (d_move < 0):
            for i in range(-1, d_move-1, -1):
                b_to_check = current_solve[0][indexes[0] + i][indexes[1]]
                if b_to_check.up_cord[0] != b_to_move.up_cord[0] or b_to_check.down_cord[0] != b_to_move.down_cord[0]: # Jesli mamy juz shift na y w danej kolumnie to nie robimy nic
                    break
            else:
                for i in range(-1, d_move-1, -1):
                    b_to_move.up_cord[1] -= (b_to_check.down_cord[1] - b_to_check.up_cord[1] + 1) # przesuwamy o k
                    b_to_move.down_cord[1] -= (b_to_check.down_cord[1] - b_to_check.up_cord[1] + 1) # przesuwamy o k

                    b_to_check = current_solve[0][indexes[0] + i][indexes[1]]
                    b_to_check.up_cord[1] += (b_to_move.down_cord[1] - b_to_move.up_cord[1] + 1)
                    b_to_check.down_cord[1] += (b_to_move.down_cord[1] - b_to_move.up_cord[1] + 1)
                    current_solve[0][indexes[0] + i][indexes[1]] = b_to_move
                    current_solve[0][indexes[0] + i+1][indexes[1]] = b_to_check
                indexes[0] += d_move
                is_same = False
    
    return current_solve, is_same


def probability(d_f, T):
    return e**(-(d_f/T))