#include "main.h"

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
        s = louvain(&reordered, igraph_vcount(&reordered));
        igraph_destroy(&reordered);
    } else {
        s = louvain(main_component, igraph_vcount(main_component));
    }
    
    save(graph_path, s, reorder_id, node_type);
    
    igraph_destroy(&graph);
    igraph_destroy(main_component);
    
    return 0;
}
