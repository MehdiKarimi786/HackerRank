You are given an integer . A set, , of triples  is beautiful if and only if:

Let  be the set of different 's in ,  be the set of different 's in , and  be the set of different  in . Then .
The third condition means that all 's are pairwise distinct. The same goes for  and .

Given , find any beautiful set having a maximum number of elements. Then print the cardinality of  (i.e., ) on a new line, followed by  lines where each line contains  space-separated integers describing the respective values of , , and .

Input Format

A single integer, .

Constraints

Output Format

On the first line, print the cardinality of  (i.e., ).
For each of the  subsequent lines, print three space-separated numbers per line describing the respective values of , , and  for triple  in .

Sample Input

3
Sample Output

3
0 1 2
2 0 1
1 2 0
Explanation

In this case, . We need to construct a set, , of non-negative integer triples () where .  has the following triples:

We then print the cardinality of this set, , on a new line, followed by  lines where each line contains three space-separated values describing a triple in S.