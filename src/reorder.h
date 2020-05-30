#ifndef reorder_h
#define reorder_h

#include <stdio.h>
#include <time.h>
#include <igraph.h>
#include <err.h>

// apply bfs reordering with root as the nen graph's root
// returns reorder duration
float reorder(igraph_t *graph, igraph_t *reordered_graph, igraph_integer_t root, int nbVertex);

#endif /* reorder_h */
