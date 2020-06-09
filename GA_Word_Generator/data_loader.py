def load_stdin():
    first_line = input().split(" ")
    t = float(first_line[0])
    n = int(first_line[1])
    s = int(first_line[2])
    letters = {}
    letters_counter = {}
    letters_list = []
    for _ in range(n):
        line = input().split(" ")
        letters_list += [line[0]]
        if line[0] in letters_counter:
            letters_counter[line[0]] += 1
        else:
            letters_counter[line[0]] = 1

        if line[1][-1] == "\n":
            letters[line[0]] = int(line[1][:-1])
        else:
            letters[line[0]] = int(line[1])
    x = []
    for _ in range(s):
        line = input()
        if line[-1] == "\n":
            x += [list(line[:-1])]
        else:
            x += [list(line)]
    return t, letters, letters_counter, x, letters_list


def load_data(path):
    ''' Fast Loading and Finding in O(1)
    set hashtable'''
    dic = set()
    with open(path) as f:
        for lines in f:
            if lines[-1] == "\n":
                dic.add(lines[:-1].lower())
            else:
                dic.add(lines.lower())
    return dic
