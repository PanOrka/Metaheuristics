from statistics import mean


class block:
    def __init__(self, up_cord, down_cord): # cord = [x, y]
        self.up_cord = up_cord
        self.down_cord = down_cord
        self.avg = 0
    
    def calc_avg(self, M, values = [0, 32, 64, 128, 160, 192, 223, 255]):
        m_val = []
        for i in range(self.down_cord[0] - self.up_cord[0] + 1):
            for j in range(self.down_cord[1] - self.up_cord[1] + 1):
                m_val += [ M[j+self.up_cord[1]][i+self.up_cord[0]] ]
        self.avg = min(values, key=lambda x:abs(x-mean(m_val)))
    
    def __str__(self):
        return str((self.up_cord, self.down_cord))


def create_blocks_RD(n, m, k): # DONE
    blocks = []

    for i in range((n // k) - 1):
        blocks += [[ block([j*k, i*k], [(j+1)*k - 1, (i+1)*k - 1]) for j in range((m // k) - 1) ] + [ block( [((m//k)-1)*k, i*k] , [m-1, (i+1)*k - 1] ) ]]
    
    blocks += [[ block( [j*k, ((n // k) - 1)*k] , [(j+1)*k - 1, n-1] ) for j in range((m // k) - 1)] + [ block( [((m//k)-1)*k, ((n // k) - 1)*k], [m-1, n-1] )]]

    special_blocks_R = [[i, len(blocks[0]) - 1] for i in range((n // k) - 1)] # tutaj domyslnie [y, x]
    special_blocks_D = [[len(blocks)-1, i] for i in range((m // k))]
    special_blocks_R += [special_blocks_D[-1]] # dodajemy najwiekszy blok zeby mogl sie przesuwac vertical i horizontal

    return blocks, special_blocks_R, special_blocks_D


def create_blocks_RU(n, m, k): # DONE
    blocks = []

    blocks += [[ block([i*k, 0], [(i+1)*k - 1, k+n%k-1]) for i in range((m // k) - 1) ] + [ block([((m//k)-1)*k, 0], [m-1, k+n%k-1]) ]]

    for i in range(1, (n // k)):
        blocks += [[ block([j*k, i*k + n%k], [(j+1)*k - 1, (i+1)*k - 1 + n%k]) for j in range((m // k) - 1) ] + [ block( [((m//k)-1)*k, i*k + n%k] , [m-1, (i+1)*k - 1 + n%k] ) ]]

    special_blocks_R = [[i, len(blocks[0]) - 1] for i in range(1, (n // k))] # tutaj domyslnie [y, x]
    special_blocks_D = [[0, i] for i in range((m // k))]
    special_blocks_R += [special_blocks_D[-1]] # dodajemy najwiekszy blok zeby mogl sie przesuwac vertical i horizontal

    return blocks, special_blocks_R, special_blocks_D


def create_blocks_LU(n, m, k): # DONE
    blocks = []

    blocks += [[ block([0, 0], [k+m%k-1, k+n%k-1]) ] + [ block([i*k+m%k, 0], [(i+1)*k - 1+m%k, k+n%k-1]) for i in range(1, (m // k)) ]]

    for i in range(1, (n // k)):
        blocks += [[ block( [0, i*k+n%k] , [k+m%k-1, (i+1)*k - 1 + n%k] ) ] + [ block([j*k+m%k, i*k + n%k], [(j+1)*k - 1+m%k, (i+1)*k - 1 + n%k]) for j in range(1, (m // k)) ]]

    special_blocks_R = [[0, 0]] # tutaj domyslnie [y, x]
    special_blocks_D = [[0, i] for i in range((m // k))]
    special_blocks_R += [[i, 0] for i in range(1, (n // k))] # dodajemy najwiekszy blok zeby mogl sie przesuwac vertical i horizontal

    return blocks, special_blocks_R, special_blocks_D


def create_blocks_LD(n, m, k): # DONE
    blocks = []

    for i in range((n // k) - 1):
        blocks += [[ block( [0, i*k] , [k+m%k-1, (i+1)*k-1] ) ] + [ block([j*k+m%k, i*k], [(j+1)*k - 1+m%k, (i+1)*k - 1]) for j in range(1, (m // k)) ]]
    
    blocks += [[ block( [0, ((n // k) - 1)*k], [k+m%k-1, n-1] )] + [ block( [j*k+m%k, ((n // k) - 1)*k] , [(j+1)*k - 1+m%k, n-1] ) for j in range(1, (m // k))]]

    special_blocks_R = [[i, 0] for i in range((n // k) - 1)] # tutaj domyslnie [y, x]
    special_blocks_D = [[len(blocks)-1, i] for i in range((m // k))]
    special_blocks_R += [special_blocks_D[0]] # dodajemy najwiekszy blok zeby mogl sie przesuwac vertical i horizontal

    return blocks, special_blocks_R, special_blocks_D


# if __name__ == "__main__":
#     blocks, special_blocks_R, special_blocks_D = create_blocks_LD(16, 16, 3)
#     from matrix_functionality import create_from_blocks
#     import numpy
#     aprox_M = [[0 for _ in range(16)] for _ in range(16)]
#     for i in range(len(blocks)):
#         for k in range(len(blocks[0])):
#             blocks[i][k].avg = i*len(blocks) + k
#     create_from_blocks(aprox_M, blocks, 16, 16)
#     print(numpy.matrix(aprox_M))
#     for x in blocks:
#         print([str(b) for b in x])
#     print(special_blocks_D)
#     print(special_blocks_R)