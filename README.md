# Clustering & Reordering

## Prerequisites:

Make sure to have the igraph library. ([installation guide](https://igraph.org/c/))


## Installation:

``` sh

sh$ make
gcc -Wall -Werror -Wextra -std=c99 -pedantic -fsanitize=address -g -I/usr/local/include/igraph    -c -o src/main.o src/main.c
gcc -Wall -Werror -Wextra -std=c99 -pedantic -fsanitize=address -g -I/usr/local/include/igraph    -c -o src/clustering.o src/clustering.c
gcc -Wall -Werror -Wextra -std=c99 -pedantic -fsanitize=address -g -I/usr/local/include/igraph    -c -o src/reorder.o src/reorder.c
gcc -Wall -Werror -Wextra -std=c99 -pedantic -fsanitize=address -g -I/usr/local/include/igraph    -c -o src/utils.o src/utils.c
gcc -Wall -Werror -Wextra -std=c99 -pedantic -fsanitize=address -g -I/usr/local/include/igraph  -L/usr/local/lib -ligraph  ./src/main.o ./src/clustering.o ./src/reorder.o ./src/utils.o -o ./vlg

```


## Usage:

``` sh

sh$ ./vlg graph_path [vertex_reorder_id vertex_type]

```

Where _’vertex_reorder_id’_ is the id of the vertex used asthe reordering root and _’vertex_type’_ is the type of this node (center, peripheral, min_degree, max_degree, ...)

_**Note:**_ _'vertex_reorder_id'_ and _'vertex_type'_ are optional. If they are not given, the program will run Louvain without reordering.


## Example:

### run on inet graph without reordering

``` sh
sh$ ./vlg main_comp/inet 
[info] Loading main_comp/inet
[info] Loaded main_comp/inet
[info] Computing main component
[info] Computed main component
[info] Applying Louvain
[result] louvain time: 55.058344
[result] modularity: 0.710001
[result] number of communities: 446
[info] Saving stats in records.csv
```


### run on inet graph with reordering at center node 5

``` sh
sh$ ./vlg main_comp/inet 5 center
[info] Loading main_comp/inet
[info] Loaded main_comp/inet
[info] Computing main component
[info] Computed main component
[info] Applying reorder at index : 5
[info] Reorder time: 8.356778
[info] Applying Louvain
[result] louvain time: 80.718625
[result] modularity: 0.725551
[result] number of communities: 197
[info] Saving stats in records.csv
```


## Authors:

- Yassir RAMDANI ( yassir.ramdani@epita.fr )
- Mamoune EL HABBARI ( mamoune.el-habbari@epita.fr )
