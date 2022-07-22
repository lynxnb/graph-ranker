# GraphRanker
Individual project for _Algorithms and Principles of Computer Science_ course at [Politecnico di Milano](https://www.polimi.it/)

Grade: 30/30 Cum Laude

## Project Assignment
The assignment consists of writing a C program that manages a ranking of 
[weighted](https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)#Weighted_graph) 
[directed graphs](https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)#Directed_graph). 
Given two blank-separated integers `d` and `k` which are the number of nodes in a graph and the ranking size, respectively, 
the program has to print the top `k` graphs it has seen since the beginning of the execution, based on ranking metrics defined below.
Every graph is labeled with a number representing the amount of graphs that have been read before it, starting from `0`.

The program accepts two commands:
- `AggiungiGrafo` [adjacency matrix]: add a graph to those considered to draw up the ranking

E.g.:
```
AggiungiGrafo
3,7,42
0,7,2
7,4,3
```
- `TopK`: print the labels of the top `k` graphs received until that point, on a single line, blank-separated, in any order

Graphs have to be ranked based on the following metrics:
- Sum of all minimum-length paths from node `0` to all nodes reachable from `0` shall be minimum
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
The ranking is implemented as a Max-Heap, which is stored as an array starting from index 1 (0th position is ignored). 
Given `i` the index of the current node, its left and right children are respectively at indexes `2*i` and `2*i+1`, 
while its parent is at index `i/2`. These are then compiled as shift left/right cpu instructions which result in
very quick indexes calculations.

Every graph is read as a matrix and gets assigned a score based on the above metrics, which is then used to determine 
its position in the ranking.
The score is calculated using the Dijkstra algorithm, which has a time complexity of Θ(V<sup>2</sup> + E) and a space 
complexity of Θ(V + E), where `V` is the number of nodes and `E` represent the number of links.
