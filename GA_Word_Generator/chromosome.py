def fitness(generation, letters):
    for chromosome in generation:
        chromosome.value = 0
        for letter in chromosome.word:
            chromosome.value += letters[letter]


def fitness_one(chromosome, letters):
    for letter in chromosome.word:
        chromosome.value += letters[letter]


class chromosome:
    def __init__(self, word):
        self.word = word
        self.value = 0
