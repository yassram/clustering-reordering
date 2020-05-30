#include "clustering.h"

struct stats louvain(igraph_t *graph, int nbVertex) {
    printf("[info] Applying Louvain\n");
    clock_t start, end;
    double duration;
    
    igraph_vector_t membership;
    igraph_vector_t modularity;
    
    igraph_vector_init(&membership, nbVertex);
    igraph_vector_init(&modularity, 0);
    
    
    // start
    start = clock();
    int error = igraph_community_multilevel(graph,
                                            NULL,
                                            &membership,
                                            NULL,
                                            &modularity);
    end = clock();
    // end
    
    if (error)
        err(120, "Louvain failed!");
    
    duration = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("[result] louvain time: %f\n", duration);
    
    int modularity_index = igraph_vector_size(&modularity) - 1;
    float modularity_value = VECTOR(modularity)[modularity_index];
    printf("[result] modularity: %f\n", modularity_value);
    
    int communities_number = igraph_vector_max(&membership) + 1;
    printf("[result] number of communities: %d\n", communities_number);
    
    
    igraph_vector_destroy(&membership);
    
    struct stats s;
    s.duration = duration;
    s.modularity = modularity_value;
    s.communities_number = communities_number;
    
    return s;
}
