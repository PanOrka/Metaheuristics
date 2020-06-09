import data_loader
from time import process_time
from chromosome import chromosome, fitness
from random import shuffle, randint
from reproduction import reproduce, reproduce_strong, reproduce_weak, reproduce_random, mutate


def generate_population(x_start, letters, found_word):
    generation = []

    for x in x_start:
        to_str = ''.join(x)
        if to_str not in found_word:
            generation += [chromosome(x)]
            found_word.add(to_str)
    for letter in letters:
        if letter not in found_word:
            generation += [chromosome(list(letter))]
            found_word.add(letter)
    return generation


def clear(generation, population, dic, letters_counter, letters, found_word):
    counter = 0
    while counter < len(generation):
        if len(generation) <= population:
            return
        to_str = ''.join(generation[counter].word)
        if to_str not in dic:
            del generation[counter]
            found_word.remove(to_str)
        else:
            for letter in letters:
                if generation[counter].word.count(letter) > letters_counter[letter]: # better count method
                    del generation[counter]
                    found_word.remove(to_str)
                    break
            else:
                counter += 1
    while len(generation) > population:
        found_word.remove(''.join(generation[0].word))
        del generation[0]


def clear_all(generation, dic, letters):
    counter = 0
    while counter < len(generation):
        if ''.join(generation[counter].word) not in dic:
            del generation[counter]
        else:
            for letter in letters:
                if generation[counter].word.count(letter) > letters_counter[letter]: # better count method
                    del generation[counter]
                    break
            else:
                counter += 1


def create_new_population(generation, add_max_perm, low_bound, max_bound, found_word, population):
    if low_bound != max_bound:
        for _ in range(population):
            shuffle(add_max_perm)
            i = randint(low_bound, max_bound)
            temp = chromosome(add_max_perm[:i].copy())

            to_str = ''.join(temp.word)
            if to_str not in found_word:
                generation += [temp]
                found_word.add(to_str)


def optimize(t, letters, letters_counter, dic, x_start, letters_list,
             population, weak_sel, strong_sel, mutate_prob, take_prob):
    t_start = process_time()
    found_word = set() # fast hashing table O(1) on Average
    generation = generate_population(x_start, letters, found_word)

    clear_treshold = 5
    clear_counter = 0
    
    add_max_perm = letters_list
    shuffle(add_max_perm)
    to_str = ''.join(add_max_perm)
    if to_str not in found_word:
        generation += [chromosome(add_max_perm.copy())]
        found_word.add(to_str)

    low_bound = int(0.1*len(add_max_perm))
    max_bound = int(0.75*len(add_max_perm))

    create_new_population(generation, add_max_perm, low_bound, max_bound, found_word, population)

    while process_time() - t_start <= t:
        mutate(generation, letters_list, mutate_prob, found_word)

        #
        # NEEDED FOR ELITISM AND WEAK
        #
        # fitness(generation, letters)
        # generation.sort(key=(lambda chromosome: chromosome.value))

        #
        # ELITISM
        #
        # reproduce_strong(generation, int(strong_sel*len(generation)), found_word)

        #
        # ALLOW WEAK
        #
        # reproduce_weak(generation, int(weak_sel*len(generation)), found_word)

        #
        # RANDOM (BEST SO FAR)
        #
        reproduce_random(generation, take_prob, found_word)

        fitness(generation, letters)
        generation.sort(key=(lambda chromosome: chromosome.value))

        #
        # CLEAR
        #
        if clear_counter >= clear_treshold:
            clear(generation, population, dic, letters_counter, letters, found_word)
            clear_counter = 0

            create_new_population(generation, add_max_perm, low_bound, max_bound, found_word, population)

            shuffle(add_max_perm)
            to_str = ''.join(add_max_perm)
            if to_str not in found_word:
                generation += [chromosome(add_max_perm.copy())]
                found_word.add(to_str)
        else:
            clear_counter += 1
        #
        #
        #
    fitness(generation, letters)
    generation.sort(key=(lambda chromosome: chromosome.value))
    clear_all(generation, dic, letters)
    for x in generation:
        print(''.join(x.word), x.value)
    return generation[-1]


if __name__ == "__main__":
    t, letters, letters_counter, x, letters_list = data_loader.load_stdin()
    dic = data_loader.load_data("./test.txt")
    best_x = optimize(t, letters, letters_counter, dic, x, letters_list,
                      20, 0.1, 0.2, 0.5, 0.3)  # population, weak_sel, strong_sel, mutate_prob, take_prob
    print(best_x.word, best_x.value)
