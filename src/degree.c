#include "degree.h"

void get_max_min_degree(igraph_t *graph) {
    igraph_vector_t degrees;
    igraph_vector_init(&degrees, 0);
    
    igraph_degree(graph, &degrees,
                  igraph_vss_all(),
                  IGRAPH_ALL, 0);
    
    int min_deg = 2147483647;
    int max_deg = -1;
    int limit_min = 5;
    int limit_max = 5;
    
    for (int i = 0; i < igraph_vector_size(&degrees); ++i)
        {
        if (VECTOR(degrees)[i] > max_deg) {
            max_deg = VECTOR(degrees)[i];
            limit_max = 5;
        }
        
        if (VECTOR(degrees)[i] < min_deg) {
            min_deg = VECTOR(degrees)[i];
            limit_min = 5;
        }
        
        if (VECTOR(degrees)[i] == max_deg && limit_max > 0) {
            printf("x current_max[%d] v : %d\n", max_deg, i);
            --limit_max;
        }
        
        if (VECTOR(degrees)[i] == min_deg && limit_min > 0) {
            printf("> current_min[%d] v : %d\n", min_deg, i);
            --limit_min;
        }
        
        }
    
    igraph_vector_destroy(&degrees);
}
