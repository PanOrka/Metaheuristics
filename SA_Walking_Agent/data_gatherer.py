def get_data():
    frst_line = str(input())
    lst = frst_line.split(" ")
    max_sec = float(lst[0])
    n = int(lst[1])
    m = int(lst[2])
    agent_pos = []
    crate = []
    for i in range(n):
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
