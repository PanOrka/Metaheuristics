from random import randint, choice, uniform, shuffle
from chromosome import chromosome
from perturb_func import swap_1, swap_2, swap_3, cut_2, cut, insert_at_end, insert_random_place, insert_at_beg


def reproduce(x, y):
    if len(x.word) >= 4 and len(y.word) >= 4:
        cut_type = randint(0, 1)
    else:
        cut_type = 0

    if len(x.word) != len(y.word):
        new_len = randint(0, 1)
        if new_len == 0:
            put = x.word
            take = y.word
        else:
            put = y.word
            take = x.word
        new_word = put.copy()
        if cut_type == 0:
            if len(take) > len(new_word):
                start = randint(0, len(new_word)-2)
                end = randint(start, len(new_word)-1)
                l = end - start + 1
                start_2 = randint(0, len(take)-l)
                for i in range(l):
                    new_word[start+i] = take[start_2+i]
            else:
                start = randint(0, len(take)-2)
                end = randint(start, len(take)-1)
                l = end - start + 1
                start_2 = randint(0, len(new_word)-l)
                for i in range(l):
                    new_word[start_2+i] = take[start+i]
        else:
            if len(take) > len(new_word):
                start = randint(1, len(new_word)-3)
                end = randint(start, len(new_word)-2)
                frst_l = start
                last_l = len(new_word)-end-1
                start_1 = randint(0, len(take)-frst_l-last_l)
                start_2 = randint(start_1, len(take)-last_l)
                for i in range(frst_l):
                    new_word[i] = take[start_1+i]
                for i in range(last_l):
                    new_word[end+i] = take[start_2+i]
            else:
                start = randint(1, len(take)-3)
                end = randint(start, len(take)-2)
                frst_l = start
                last_l = len(take)-end-1
                start_1 = randint(0, len(new_word)-frst_l-last_l)
                start_2 = randint(start_1, len(new_word)-last_l)
                for i in range(frst_l):
                    new_word[start_1+i] = take[i]
                for i in range(last_l):
                    new_word[start_2+i] = take[end+i]
        return chromosome(new_word)
    else:
        new_len = randint(0, 1)
        if new_len == 0:
            put = x.word
            take = y.word
        else:
            put = y.word
            take = x.word
        new_word = put.copy()
        if cut_type == 0:
            start = randint(0, len(new_word)-2)
            end = randint(start, len(new_word)-1)
            l = end - start + 1
            for i in range(l):
                new_word[start+i] = take[start+i]
        else:
            start = randint(1, len(new_word)-3)
            end = randint(start, len(new_word)-2)
            frst_l = start
            last_l = len(new_word)-end-1
            for i in range(frst_l):
                new_word[i] = take[i]
            for i in range(last_l):
                new_word[end+i] = take[end+i]
        return chromosome(new_word)


def reproduce_weak(generation, weak_sel, found_word):
    if len(generation) >= 2:
        if weak_sel%2 != 0:
            if len(generation) >= weak_sel+1:
                weak_sel += 1
            else:
                weak_sel -= 1
        sec_list = generation[:weak_sel]
        shuffle(sec_list)
        for i in range(0, len(sec_list)-2, 2):
            if len(sec_list[i].word) > 2 and len(sec_list[i+1].word) > 2:
                child = reproduce(sec_list[i], sec_list[i+1])
                child_str = ''.join(child.word)
                if child_str not in found_word:
                    found_word.add(child_str)
                    generation += [child]


def reproduce_strong(generation, strong_sel, found_word):
    if len(generation) >= 2:
        if strong_sel%2 != 0:
            if len(generation) >= strong_sel+1:
                strong_sel += 1
            else:
                strong_sel -= 1
        sec_list = generation[len(generation)-strong_sel:]
        shuffle(sec_list)
        for i in range(0, len(sec_list)-2, 2):
            if len(sec_list[i].word) > 2 and len(sec_list[i+1].word) > 2:
                child = reproduce(sec_list[i], sec_list[i+1])
                child_str = ''.join(child.word)
                if child_str not in found_word:
                    found_word.add(child_str)
                    generation += [child]


def reproduce_random(generation, take_prob, found_word):
    if len(generation) >= 2:
        pair = []
        shuffle(generation)
        for x in generation:
            if uniform(0, 1) <= take_prob:
                pair += [x]
            if len(pair) == 2:
                if len(pair[0].word) > 2:
                    if len(pair[1].word) > 2:
                        child = reproduce(pair[0], pair[1])
                        child_str = ''.join(child.word)
                        if child_str not in found_word:
                            found_word.add(child_str)
                            generation += [child]
                    else:
                        del pair[1]
                else:
                    del pair[0]

def mutate(generation, letters_list, mutate_prob, found_word):
    func_list = (swap_1, swap_2, swap_3, cut_2, cut, insert_at_end, insert_random_place, insert_at_beg)
    mutated = []
    for x in generation:
        if uniform(0, 1) <= mutate_prob:
            new = chromosome(x.word.copy())
            choice(func_list)(new, letters_list)
            str_new = ''.join(new.word)
            if str_new not in found_word:
                found_word.add(str_new)
                mutated += [new]

    generation += mutated
