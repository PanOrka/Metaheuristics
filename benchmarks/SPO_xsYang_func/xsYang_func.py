from random import uniform
from time import process_time


def func(x, e):
    sum = 0.0
    for i in range(len(x)):
        sum += e[i]*(abs(x[i])**(i+1))
    return sum


def take_stdin():
    data = input().split(" ")
    start_x = []
    e = []
    t = float(data[0])
    for i in range(1, 6):
        start_x += [float(data[i])]
    for i in range(6, 11):
        e += [float(data[i])]
    return t, start_x, e


class swarm_particle:
    def __init__(self, b_low, b_high, position=None):
        if position is None:
            self.position = list( (uniform(b_low, b_high),
                                   uniform(b_low, b_high),
                                   uniform(b_low, b_high),
                                   uniform(b_low, b_high),
                                   uniform(b_low, b_high)) )
        else:
            self.position = position

        self.best_position = self.position
        self.best_value = 0.0

        d_set = b_high - b_low
        self.velocity = list( (uniform(-abs(d_set), abs(d_set)),
                               uniform(-abs(d_set), abs(d_set)),
                               uniform(-abs(d_set), abs(d_set)),
                               uniform(-abs(d_set), abs(d_set)),
                               uniform(-abs(d_set), abs(d_set))) )


def create_swarm(population, b_low, b_high, start_x, e):
    swarm_p = swarm_particle(b_low, b_high, start_x)
    swarm_p.best_value = func(swarm_p.position, e)
    best_value, best_vec = swarm_p.best_value, swarm_p.position

    swarm = [swarm_p]
    for _ in range(population-1):
        swarm_p = swarm_particle(b_low, b_high)
        swarm_p.best_value = func(swarm_p.position, e)
        swarm += [swarm_p]

        if swarm_p.best_value < best_value:
            best_value, best_vec = swarm_p.best_value, swarm_p.position

    return swarm, best_value, best_vec


def optimize(population, b_low, b_high, t, start_x, e,
             w, o_p, o_g):
    swarm, best_value, best_vec = create_swarm(population, b_low, b_high, start_x, e)

    t_start = process_time()
    while process_time() - t_start <= t:
        for p in swarm:
            for i in range(len(p.position)):
                r_p = uniform(0, 1)
                r_g = uniform(0, 1)
                #
                # VELOCITY update
                #
                p.velocity[i] = (w*p.velocity[i] +
                                 o_p*r_p*(p.best_position[i]-p.position[i]) +
                                 o_g*r_g*(best_vec[i]-p.position[i]))
                #
                # POSITION update (movement)
                #
                p.position[i] += p.velocity[i]
            ac_value = func(p.position, e)
            if ac_value < p.best_value:
                p.best_value, p.best_position = ac_value, p.position
                if p.best_value < best_value:
                    best_value, best_vec = p.best_value, p.best_position
    return best_vec, best_value


if __name__ == "__main__":
    t, start_x, e = take_stdin()
    x, y = optimize(1000, -5, 5, t, start_x, e,
                    0.1, 0.1, 0.1)
    print(x, y)
