#include "utils.h"

void save(const char *graph, struct stats s, int reorder_id, const char *node_type) {
    printf("[info] Saving stats in records.csv\n");
    FILE *file = fopen("records.csv", "a");
    fprintf(file, "%s,%f,%f,%d,%d,%s\n",graph ,s.duration, s.modularity, s.communities_number, reorder_id, node_type);
    fclose(file);
}

int load_graph(igraph_t *graph, const char* fileName,
               igraph_bool_t isDirected) {
    printf("[info] Loading %s\n", fileName);
    
    FILE *graphFile;
    graphFile = fopen(fileName, "r");
    
    size_t s;
    char *l = NULL;
    ssize_t sizeOfFirstLine = getline(&l,  &s, graphFile);
    int nbVertex = atoi(l);
    free(l);
    
    fseek(graphFile, sizeOfFirstLine, SEEK_SET);
    
    if (graphFile == NULL)
        err(1, "Couldn't read %s\n", fileName);
    
    igraph_read_graph_edgelist(graph, graphFile, 0, isDirected);
    printf("[info] Loaded %s\n", fileName);
    return nbVertex;
}

void get_main_component(igraph_t *graph, igraph_t **main_component) {
    printf("[info] Computing main component\n");
    
    igraph_vector_ptr_t components;
    igraph_vector_ptr_init(&components, 0);
    
    
    int error = igraph_decompose(graph,
                                 &components,
                                 IGRAPH_WEAK,
                                 -1,
                                 2);
    if (error) {
        igraph_vector_ptr_destroy(&components);
        err(120, "Decompose failed!");
    }
    
    *main_component = VECTOR(components)[0];
    for (int i = 1; i < igraph_vector_ptr_size(&components); ++i)
        {
        igraph_t *component = VECTOR(components)[i];
        if (igraph_vcount(*main_component) < igraph_vcount(component)) {
            igraph_destroy(*main_component);
            *main_component = component;
        }
        }
    
    igraph_vector_ptr_destroy(&components);
    
    printf("[info] Computed main component\n");
}
