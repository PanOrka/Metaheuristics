# **Travelling Salesman Problem with Tabu Search**
Here is my first attempt in metaheuristics to make a model that searches for the shortest path on given graph 
going from town #1 through every town and back to town #1.<br>
It's not metric TSP problem, we have to give distance from a to b and from b to a, it's asymetric (unless said so).<br>
Here i use **Tabu Search** with fixed queue based on history of permutation changes.<br><br>
Tabu list is made with FIFO queue based on ![**Two way circular list**](url) from my repo.<br><br>
For random permutation I used permuted congruential generator, files: pcg_basic.c and pcg_basic.h were downloaded from 
site: https://www.pcg-random.org/ and are not mine.<br><br>
After start program waits for data on stdin, first line is: **t** **n**.<br>Where **t** is runtime of algorithm and **n** is 
number of towns. After this we put n lines where we define distance from town to other towns in order.<br>
Example input:<br>
```
1 5
0 1 5 1 6
1 0 5 1 100
7 20 0 4 21
1 7 4 0 11
50 50 55 50 0
```
in line **k** is 0 in **k**-th place, we assume that distance from town **k** to town **k** is 0.<br>
After execution in given runtime program gives on stdout lowest distance that algorithm has found and next line is 
best found cycle.<br>
Example output:<br>
```
66
1 5 2 3 4 1
```
