#include "main.h"


struct stats
{
    float duration;
    float modularity;
    int communities_number; 
};

int load_graph(igraph_t *graph, const char* fileName, 
               igraph_bool_t isDirected) {
    
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
    
    printf("Loading %s\n", fileName);
    igraph_read_graph_edgelist(graph, graphFile, 0, isDirected);
    printf("Loaded %s\n", fileName);
    return nbVertex;
}

struct stats louvain(igraph_t *graph, int nbVertex) {
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
    printf("louvain time: %f\n", duration);
    
    int modularity_index = igraph_vector_size(&modularity) - 1;
    float modularity_value = VECTOR(modularity)[modularity_index];
    printf("modularity: %f\n", modularity_value);
    
    int communities_number = igraph_vector_max(&membership) + 1;
    printf("number of communities: %d\n", communities_number);
    
    
    igraph_vector_destroy(&membership);
    
    struct stats s;
    s.duration = duration;
    s.modularity = modularity_value;
    s.communities_number = communities_number;
    
    return s;
}



int peripheral_points_get(igraph_t *graph, igraph_vector_t *peripheral_points) {
    
    igraph_integer_t diameter = 0;
    igraph_vector_t eccentricity;
    igraph_vector_init(&eccentricity, 0);
    
    int eccentricity_error = igraph_eccentricity(graph, &eccentricity, igraph_vss_all(), IGRAPH_ALL);
    
    int diameter_error = igraph_diameter(graph, &diameter, NULL, NULL, 0, IGRAPH_ALL, 1);
    
    for (long int i = 0; i < igraph_vcount(graph); i++)
        {
        if (VECTOR(eccentricity)[i] == diameter)
            {
            igraph_vector_push_back(peripheral_points, i);
            }
        }
    
    return eccentricity_error + diameter_error;
}


float reorder(igraph_t *graph, igraph_t *reordered_graph, igraph_integer_t root, int nbVertex) {
    
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
    printf("reorder time: %f\n", duration);
    
    igraph_vector_destroy(&order);
    
    return duration;
}


void get_main_component(igraph_t *graph, igraph_t **main_component) {
    
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
    
}

void save(const char *graph, struct stats s, int reorder_id, const char *node_type) {
    FILE *file = fopen("records.csv", "a");
    fprintf(file, "%s, %f, %f, %d, %d, %s\n",graph ,s.duration, s.modularity, s.communities_number, reorder_id, node_type);
    fclose(file);
}


int main(int argc, char const *argv[])
{
    if (argc < 2) 
        return 1;
    
    int reorder_id = -1; 
    const char *node_type = "-";
    
    if (argc == 4) {
        reorder_id = atoi(argv[2]);
        node_type = argv[3];
    }
    
    const char *graph_path = argv[1];
    
    igraph_t graph;
    load_graph(&graph, graph_path, 0);
    
    igraph_t *main_component;
    get_main_component(&graph, &main_component);

     igraph_t reordered;
     struct stats s;
     if (reorder_id != -1) {
         reorder(main_component, &reordered, reorder_id, igraph_vcount(main_component));
         s = louvain(&reordered, igraph_vcount(main_component));
         igraph_destroy(&reordered);
     } else {
         s = louvain(main_component, igraph_vcount(main_component));
     }
    
     save(graph_path, s, reorder_id, node_type);
    
    igraph_destroy(&graph);
    igraph_destroy(main_component);
    
    return 0;
}
