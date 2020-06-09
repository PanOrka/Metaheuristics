from random import randint, choice, uniform
from chromosome import chromosome


def swap_1(chrom, letters_list):
    k = randint(0, len(chrom.word)-1)
    chrom.word[k] = choice(letters_list)


def swap_2(chrom, letters_list):
    k = randint(0, len(chrom.word)-1)
    j = randint(0, len(chrom.word)-1)
    temp = chrom.word[k]
    chrom.word[k] = chrom.word[j]
    chrom.word[j] = temp


def swap_3(chrom, letters_list):
    k = randint(0, len(chrom.word)-1)
    j = randint(0, len(chrom.word)-1)
    l = randint(0, len(chrom.word)-1)
    temp = chrom.word[k]
    chrom.word[k] = chrom.word[j]
    chrom.word[j] = chrom.word[l]
    chrom.word[l] = temp


def cut_2(chrom, letters_list):
    if len(chrom.word) > 4:
        k = randint(0, len(chrom.word)-2)
        del chrom.word[k]
        del chrom.word[k]


def cut(chrom, letters_list, prob=0.1, c=uniform(1, 5)):
    p=1
    while len(chrom.word) >= 3:
        if uniform(0, 1) <= p:
            k = randint(0, len(chrom.word)-1)
            del chrom.word[k]
            p *= prob
            p *= c
            c = uniform(1, 5)
        else:
            break


def insert_at_end(chrom, letters_list, prob=0.1, c=uniform(1, 8)):
    p = 1
    while uniform(0, 1) <= p:
        chrom.word += [choice(letters_list)]
        p *= prob
        p *= c
        c = uniform(1, 8)


def insert_random_place(chrom, letters_list, prob=0.1, c=uniform(1, 8)):
    p = 1
    while uniform(0, 1) <= p:
        i = randint(0, len(chrom.word))
        chrom.word.insert(i, choice(letters_list))
        p *= prob
        p *= c
        c = uniform(1, 8)


def insert_at_beg(chrom, letters_list, prob=0.1, c=uniform(1, 8)):
    p = 1
    while uniform(0, 1) <= p:
        chrom.word.insert(0, choice(letters_list))
        p *= prob
        p *= c
        c = uniform(1, 8)
