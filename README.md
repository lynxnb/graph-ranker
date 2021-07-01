# GraphRanker
Individual project for _Algorithms and Principles of Computer Science_ course at [Politecnico di Milano](https://www.polimi.it/)

## Project Goal
The assignment consists of writing a C program that manages a ranking of weighted directed graphs. Given two blank-separated integers `d` and `k`, respectively the number of nodes in a graph and the ranking size, the program has to keep the top `k` graphs in the ranking. Every graph is assigned an index relative to number of graphs that have been read before it, starting from `0`.

The program accepts two commands:
- `AggiungiGrafo` [adjacency matrix]: add a graph to the list of graphs to be considered for the creation of the ranking

E.g.:
```
AggiungiGrafo
3,7,42
0,7,2
7,4,3
```
- `TopK`: prints the indexes of the top `k` graphs received until that point, on a single line, blank-separated, in any order

Graphs have to be ranked based on the following metrics:
- Sum of all minimum-lenght paths from node `0` to all nodes reachable from `0` shall be minimum
- Paths to nodes not reachable from node `0` are considered of zero length
- If two graphs reach the same metric value, the oldest one stays

### Execution sample
```
Input:
3,2                       3-nodes graphs, print the 2 best graphs
AggiungiGrafo             First graph added (index 0, paths sum = 7)
0,4,3
0,2,0
2,0,0
AggiungiGrafo             Second graph added (index 1, paths sum = 5)
0,0,2
7,0,4
0,1,0
AggiungiGrafo             Third graph added (index 2, paths sum = 7)
3,1,8
0,0,5
0,9,0
TopK

Output:
0 1 Or 1 0
```

## Code Description
WIP
