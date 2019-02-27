# Discrete_ShortestPath
Implementation of shortest_path problem with discrete points as visibility graph::

Input files contain: start point, end point, a list of triangles.

Problem: find the shortest path, but you cannot travel on triangles sides (!= normal visibility graph is not allowed)

|| "solver.c" contains simple heuristic algorithm to approach the goal point (euclidean distance on 2D plane), and 
|| returns the path as a list of points
