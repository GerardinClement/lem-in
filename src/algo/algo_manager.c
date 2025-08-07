#include "lem_in.h"

static void         calculate_all_rooms_cost(t_lem_in *lem_in);
static int          *reach_path(t_lem_in lem_in, t_room *room, int *visited);
static size_t       compute_distribution(t_lem_in *lem_in);
static void         backtracking_path(t_lem_in *lem_in, int *visited, t_path *path);
static void         augment_flow(t_lem_in *lem_in, int *visited, int start, int end);

const int NOT_VISITED = 0;
const int NO_PARENT = -1;
const int VISITED = 1;
const int START = -2;


static void init_visited(int *visited, int n_rooms)
{
    for (int i = 0; i < n_rooms; i++)
    {
        visited[i] = NOT_VISITED;
    }
}

static void init_parent(int *parent, int n_rooms)
{
    for (int i = 0; i < n_rooms; i++)
    {
        parent[i] = NO_PARENT;
    }
}

void    algo_manager(t_lem_in *lem_in)
{
    calculate_all_rooms_cost(lem_in);
}

static int edmonds_karp(t_lem_in *lem_in) {
    int max_flow = 0;
    int *visited;
    int *parent;
    t_room *room = &lem_in->rooms[lem_in->start];
    lem_in->n_paths = 0;
    // printf("Edmonds-Karp algorithm started.\n");

    visited = malloc(sizeof(int) * lem_in->n_rooms);
    if (!visited) {
        perror("Failed to allocate memory for visited array");
        exit(EXIT_FAILURE);
    }
    while (1) {
        init_visited(visited, lem_in->n_rooms);
        parent = reach_path(*lem_in, room, visited);
        if (!parent)
            break;

        for (int i = 0; i < lem_in->n_rooms; i++) {
            printf("Parent[%d] = %d\n", i, parent[i]);
        }
        backtracking_path(lem_in, parent, &lem_in->all_paths[max_flow]);
        augment_flow(lem_in, parent, lem_in->start, lem_in->end);
        max_flow++;
    }
    
    // for (int i = 0; i < max_flow; i++) {
    //     printf("1: Path %d: size: %ld\n", i + 1, lem_in->all_paths[i].size);
    // }
    printf("Max flow: %d\n", max_flow);
    return max_flow;
}


static size_t compute_distribution(t_lem_in *lem_in) {
    size_t T;
    int num_ants = lem_in->n_ants;

    // 1. Trouver la longueur minimale
    T = lem_in->all_paths[0].size;
    int nb_paths = 0;

    for (int i = 0; i < lem_in->n_paths; i++) {
        if (lem_in->all_paths[i].size < T)
            T = lem_in->all_paths[i].size;
        nb_paths++;
    }

    int capacities[nb_paths];

    // 2. Boucle pour trouver le T optimal
    while (1) {
        int total_capacity = 0;

        for (int i = 0; i < nb_paths; i++) {
            capacities[i] = T - lem_in->all_paths[i].size + 1;
            if (capacities[i] < 0)
                capacities[i] = 0;
            total_capacity += capacities[i];
        }

        if (total_capacity >= num_ants)
            break;

        T++;
    }

    // 3. Remplir le tableau de distribution
    int ants_left = num_ants;
    for (int i = 0; i < nb_paths; i++) {
        if (ants_left <= 0) {
            lem_in->all_paths[i].distribution = 0;
        } else {
            int to_assign = capacities[i] < ants_left ? capacities[i] : ants_left;
            lem_in->all_paths[i].distribution = to_assign;
            ants_left -= to_assign;
        }
    }

    return T;
}


static void backtracking_path(t_lem_in *lem_in, int *parent, t_path *path) {
    int     prev_node = parent[lem_in->end];
    int     i = 0;

    while (prev_node != START) {
        if (prev_node == NO_PARENT) {
            printf("No path found from start to end. room %d\n", lem_in->rooms[prev_node].id);
            return; //TODO gestion de cette erreur
        }
        t_room *room = &lem_in->rooms[prev_node];
        int current_id = room->id;
        prev_node = parent[current_id];
        i++;
    }

    int *tmp_path = malloc(sizeof(int) * (i + 2));
    if (tmp_path == NULL) {
        print_error("Memory allocation failed.\n");
        free(parent);
        return;
    }

    prev_node = parent[lem_in->end];

    tmp_path[0] = 1;
    for (int j = 1; j < i; j++)
    {
        tmp_path[j] = lem_in->rooms[prev_node].id;
        prev_node = parent[tmp_path[j]];
    }

    path->size = i;
    path->path = tmp_path;
    // free(visited);
}

static void calculate_all_rooms_cost(t_lem_in *lem_in) 
{
    lem_in->all_paths = malloc(sizeof(t_path) * lem_in->n_rooms);
    
    if (!lem_in->all_paths) {
        perror("Failed to allocate memory for all_paths");
        exit(EXIT_FAILURE);
    }
    int max_flow = edmonds_karp(lem_in);

    // printf("Max flow: %d\n", max_flow);
    if (max_flow <= 0) {
        print_error("No paths found from start to end.\n");
        free(lem_in->all_paths);
        lem_in->all_paths = NULL;
        exit(EXIT_FAILURE);
    }

    lem_in->n_paths = max_flow;
    lem_in->all_paths[max_flow].size = -1;

    int T = compute_distribution(lem_in);
    printf("T = %d\n", T);
    for (int i = 0; i < lem_in->n_paths; i++) {
        printf("Path %d: size: %ld, distribution: %d, path: ", i + 1, lem_in->all_paths[i].size, lem_in->all_paths[i].distribution);
        for (size_t j = 0; j < lem_in->all_paths[i].size; j++) {
            printf(" %d", lem_in->all_paths[i].path[j]);
        }
        printf("\n");
    }
}

static void augment_flow(t_lem_in *lem_in, int *visited, int start, int end) {
    int curr = end;
    while (curr != start) {
        int prev = visited[curr];

        printf("Augmenting flow from %d to %d\n", prev, curr);
        t_edge *e = find_edge(lem_in->rooms[prev].edges, lem_in->rooms[prev].n_edges, curr);
        e->flow++;
        e->rev->capacity = e->capacity - e->flow;

        curr = prev;
    }
}


static void look_neighbors(t_room *room, int *parent, int *visited, t_queue *queue)
{
    for (size_t i = 0; i < room->n_edges; i++)
    {
        t_edge *next_edge = &room->edges[i];
        int room_index = next_edge->to;

        if (next_edge->capacity > next_edge->flow && visited[room_index] == NOT_VISITED) {
            parent[room_index] = room->id;
            visited[room_index] = VISITED;
            enqueue(queue, room_index);
        }
    }
}


static int *reach_path(t_lem_in lem_in, t_room *room, int *visited)
{
    t_queue     *queue;
    int         *parent;
    int         current_node;
    int         target_node;


    target_node = lem_in.end;
    queue = malloc(sizeof(t_queue));
    if (!queue) {
        perror("Failed to allocate memory for queue");
        free(visited);
        exit(EXIT_FAILURE);
    }
    parent = malloc(sizeof(int) * lem_in.n_rooms);
    if (!parent) {
        perror("Failed to allocate memory for parent array");
        free(visited);
        free(queue);
        exit(EXIT_FAILURE);
    }
    init_parent(parent, lem_in.n_rooms);
    parent[lem_in.start] = START;
    init_queue(queue);
    enqueue(queue, room->id);
    
    while (!is_queue_empty(queue)) {
        int ok = dequeue(queue, &current_node);

        if (!ok)
            break;
        if (current_node == target_node)
            break;
        look_neighbors(&lem_in.rooms[current_node], parent, visited, queue);
    }

    if (parent[target_node] == NO_PARENT) {
        free(visited);
        free(queue);
        return NULL;
    }

    destroy_queue(queue);
    free(queue);
    return parent;
}
