#include "lem_in.h"

static void create_edges(t_room *room, int n_links);

void init_edges(t_lem_in *lem_in)
{
    for (int i = 0; i < lem_in->n_rooms; i++)
    {
        create_edges(&lem_in->rooms[i], get_links_size(lem_in->rooms[i].links));
    }
}

static void create_edges(t_room *room ,int n_links)
{
    size_t i;

    room->edges = malloc(sizeof(t_edge) * n_links);
    if (!room->edges) {
        print_error(ERR_MALLOC);
        exit(EXIT_FAILURE); //TODO free ect..
    }
    for (i = 0; i < (size_t)n_links; i++)
    {
        t_edge *edge = malloc(sizeof(t_edge));
        if (!edge) {
            print_error(ERR_MALLOC);
            exit(EXIT_FAILURE); //TODO free ect..
        }

        // printf("Creating edge for room %s to link %d\n", room->name, room->links[i]);
        edge->to = room->links[i];
        edge->is_empty = 1;

        room->edges[i] = *edge;
    }

    room->n_edges = i;
}




// void add_edge(t_room *rooms, int from, int to)
// {
//     t_edge *edge = malloc(sizeof(t_edge));
//     t_edge *rev  = malloc(sizeof(t_edge));

//     edge->to = to;
//     edge->capacity = 1;
//     edge->flow = 0;
//     edge->rev = rev;
//     edge->next = rooms[from].edges;
//     rooms[from].edges = edge;

//     rev->to = from;
//     rev->capacity = 0;
//     rev->flow = 0;
//     rev->rev = edge;
//     rev->next = rooms[to].edges;
//     rooms[to].edges = rev;
// }

t_edge *find_edge(t_edge *edges, size_t n_edges, int to)
{
    size_t  i = 0;

    while (i < n_edges) {
        if (edges[i].to == to)
            return &edges[i];
        i++;
    }
    return NULL;
}
