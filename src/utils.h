#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <igraph.h>
#include <err.h>

struct stats
{
    float duration;
    float modularity;
    int communities_number;
};

// saves graph's name and stats in records.csv
void save(const char *graph, struct stats s, int reorder_id, const char *node_type);

// loads edgelist graph using igrpah
int load_graph(igraph_t *graph, const char* fileName,
igraph_bool_t isDirected);

// extracts the main component of the given graph
void get_main_component(igraph_t *graph, igraph_t **main_component);

#endif /* utils_h */
