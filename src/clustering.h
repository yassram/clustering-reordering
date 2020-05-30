#ifndef clustering_h
#define clustering_h

#include <stdio.h>
#include <time.h>
#include <igraph.h>
#include <err.h>

#include "utils.h"

// compute communities of the given graph
// returns stats (duration, modularity, communities number)
struct stats louvain(igraph_t *graph, int nbVertex);

#endif /* clustering_h */
