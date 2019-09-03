# Discrete_ShortestPath
Implementation of shortest_path problem with discrete points as visibility graph::

Input files contain: start point, end point, a list of triangles.

Problem: find the shortest path, but you cannot travel on triangles sides (!= normal visibility graph is not allowed)

1)"solver.c" contains simple heuristic algorithm to approach the goal point (euclidean distance on 2D plane), and returns the path as a list of points

2)"solverWthreads.c" use the same principle, adding a second path from the end_point to start_point with 2 threads.
Then the shortest of the two is chosen and returned.

Tip: compile the code with gcc -O3 optimization flag to get the best performance, especially under windows, or it could get some time on very large files. Algorithm takes O(n^2log(n)) to compute.
