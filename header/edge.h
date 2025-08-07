#ifndef EDGE_H
# define EDGE_H

# include "lem_in.h"

void    init_edges(t_lem_in *lem_in);
t_edge *find_edge(t_edge *edges, size_t n_edges, int to);

#endif