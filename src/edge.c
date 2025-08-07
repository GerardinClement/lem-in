#include "lem_in.h"

static void add_edge(t_room *rooms, int from, int to);

void init_edges(t_lem_in *lem_in) {
    for (int i = 0; i < lem_in->n_rooms; i++) {
        for (int j = 0; j < get_links_size(lem_in->rooms[i].links); j++) {
            int link = lem_in->rooms[i].links[j];
            add_edge(lem_in->rooms, i, link);
        }
    }
}

static void add_edge(t_room *rooms, int from, int to) {
    t_edge *edge = malloc(sizeof(t_edge));
    t_edge *rev  = malloc(sizeof(t_edge));

    if (!edge || !rev) {
        print_error(ERR_MALLOC);
        exit(EXIT_FAILURE);
    }

    edge->to = to;
    edge->capacity = 1;
    edge->flow = 0;
    edge->rev = rev;

    rev->to = from;
    rev->capacity = 0;
    rev->flow = 0;
    rev->rev = edge;

    // Ajouter edge dans room[from]
    rooms[from].edges = realloc(rooms[from].edges, sizeof(t_edge) * (rooms[from].n_edges + 1));
    rooms[from].edges[rooms[from].n_edges] = *edge;
    rooms[from].n_edges++;

    rooms[to].edges = realloc(rooms[to].edges, sizeof(t_edge) * (rooms[to].n_edges + 1));
    rooms[to].edges[rooms[to].n_edges] = *rev;
    rooms[to].n_edges++;

    free(edge);
    free(rev);
}

t_edge *find_edge(t_edge *edges, size_t n_edges, int to)
{
    size_t  i = 0;

    while (i < n_edges) {
        if (edges[i].to == to)
            return &edges[i];
        i++;
    }
    printf("Edge to room %d not found. n_edges: %zu\n", to, n_edges);
    return NULL;
}
