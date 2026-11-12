Reviewer Assignment Tool (Max-Flow)

Assignment of reviewers to submissions modelled as a maximum-flow problem and solved with the Edmonds-Karp algorithm. This document describes the risk analysis module, which checks whether the assignment still works when one or more reviewers become unavailable.

Developed for the Algorithm Design (Desenho de Algoritmos) course of the Bachelor in Informatics and Computing Engineering at the Faculty of Engineering of the University of Porto (FEUP), 2025/2026.

Authors: Afonso Vilas, Guilherme Pinho

Risk analysis

An assignment is feasible when the maximum flow equals the total number of reviews required:

required flow = number of submissions × minimum reviews per submission

A reviewer (or group of reviewers) is critical if, once removed, the maximum flow falls below this value.

K = 1: single points of failure
Approach
Iterate over every reviewer.
Temporarily disable the reviewer's vertex. Instead of physically removing and re-adding edges, the BFS used by Edmonds-Karp simply ignores that vertex, which avoids rebuilding the graph.
Run Edmonds-Karp on the modified network to compute the new maximum flow.
If the maximum flow is lower than the required flow, mark the reviewer as critical.
Restore the vertex and move on to the next reviewer.
Complexity
Edmonds-Karp runs in O(V · E²), where V is the number of vertices and E the number of edges.
It is executed once per reviewer, i.e. R times, so the analysis runs in O(R · V · E²).
Since R ≤ V, the worst case is O(V² · E²), which is perfectly practical for checking a single missing reviewer.
Example

Input: 3 submissions, each requiring 1 review (required flow = 3).

Reviewer	Capacity	Can review
R1	2	S1, S2
R2	1	S1, S3
R3	1	S2, S3

Execution: without R1, only R2 and R3 remain. Every submission is still connected to an available reviewer, but the total remaining capacity is 1 + 1 = 2.

Output: 3 reviews are needed but only 2 can be provided, so the assignment becomes impossible. The algorithm correctly reports R1 as a critical reviewer.

K > 1: simultaneous failures
Approach

Testing reviewers one at a time is no longer enough. The max-flow check is combined with a combination generator:

Generate every combination of K reviewers out of the N available.
For each combination, temporarily disable all K vertices at once.
Run Edmonds-Karp to compute the new maximum flow.
If the maximum flow is lower than the required flow, mark that group as a risk group.
Restore the K vertices and move on to the next combination.
Complexity
The number of groups is the binomial coefficient C(N, K) = N! / (K! (N − K)!).
Each group requires one run of Edmonds-Karp, O(V · E²).
Total: O(C(N, K) · V · E²).
For small K, C(N, K) grows as O(N^K): polynomial in the number of reviewers, but exponential in K.
With hundreds of reviewers, even K = 3 already means hundreds of thousands to millions of Edmonds-Karp runs, which makes this brute-force approach impractical for large datasets.
Example

This example shows a network that survives any single absence (K = 1) but fails when specific pairs of reviewers are missing (K = 2), even though the remaining capacity would be enough.

Input: 2 submissions, each requiring 1 review (required flow = 2), and 4 reviewers, each with capacity 1.

Submission	Can be reviewed by
S1	R1, R2
S2	R3, R4

Execution (K = 1): every submission has two possible reviewers, so removing any single reviewer still leaves a valid assignment. The flow is always 2.

Execution (K = 2): the algorithm tests all C(4, 2) = 6 pairs. In every case, 2 reviewers remain, so the total capacity (2) still matches the required flow. However:

Missing pair	Max flow	Result
{R1, R2}	1	S1 has no available reviewer → critical
{R3, R4}	1	S2 has no available reviewer → critical
{R1, R3}, {R1, R4}, {R2, R3}, {R2, R4}	2	Valid assignment

Output: the algorithm reports {R1, R2} and {R3, R4} as critical risk groups. The failure comes from the structure of the network, not from a lack of total capacity, which is why combinatorial search must be combined with network flow when K > 1.
