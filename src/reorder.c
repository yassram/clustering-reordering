#include "reorder.h"

float reorder(igraph_t *graph, igraph_t *reordered_graph, igraph_integer_t root, int nbVertex) {
    printf("[info] Applying reorder at index : %d\n", root);
    
    clock_t start, end;
    double duration;
    
    // bfs
    igraph_vector_t order;
    igraph_vector_init(&order, nbVertex);
    
    // start
    start = clock();
    
    int error_bfs = igraph_bfs(graph,
                               root,
                               NULL,
                               IGRAPH_ALL,
                               1,
                               NULL,
                               &order,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               NULL);
    
    if (error_bfs) {
        igraph_vector_destroy(&order);
        err(130, "Bfs failed!");
    }
    
    igraph_vector_t new_order;
    igraph_vector_init(&new_order, igraph_vector_size(&order));
    
    for (int i = 0; i < igraph_vector_size(&order); ++i)
        {
        int index = VECTOR(order)[i];
        VECTOR(new_order)[index] = i;
        }
    
    int error_permute = igraph_permute_vertices(graph,
                                                reordered_graph,
                                                &new_order);
    
    end = clock();
    // end
    
    
    if (error_permute) {
        igraph_vector_destroy(&order);
        err(140, "Permute failed!");
    }
    
    duration = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("[info] Reorder time: %f\n", duration);
    
    igraph_vector_destroy(&order);
    
    return duration;
}
