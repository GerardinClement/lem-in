#include "lem_in.h"

static void         calculate_all_rooms_cost(t_lem_in *lem_in);
static int          *reach_path(t_lem_in lem_in, t_room *room);
static size_t       compute_distribution(t_lem_in *lem_in);

const int NOT_VISITED = -1;
const int START = -2;


void    algo_manager(t_lem_in *lem_in)
{
    calculate_all_rooms_cost(lem_in);
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


static void backtracking_path(t_lem_in *lem_in, int *visited, t_path *path) {
    int     prev_node = visited[lem_in->end];
    int     i = 0;

    lem_in->rooms[prev_node].ignored = 1;

    while (prev_node != START) {
        if (prev_node == NOT_VISITED) {
            print_error("No path found from start to end.\n");
            return;
        }
        int current_id = lem_in->rooms[prev_node].id;
        prev_node = visited[current_id];
        i++;
    }

    int     *tmp_path = malloc(sizeof(int) * (i + 2));
    if (tmp_path == NULL) {
        print_error("Memory allocation failed.\n");
        free(visited);
        return;
    }
    
    prev_node = visited[lem_in->end];

    tmp_path[0] = 1;
    for (int j = 1; j < i; j++)
    {
        tmp_path[j] = lem_in->rooms[prev_node].id;
        prev_node = visited[tmp_path[j]];
    }

    path->size = i;
    path->path = tmp_path;
    free(visited);
}

static void calculate_all_rooms_cost(t_lem_in *lem_in) 
{
    t_room      *room;
    int         i = 0;
    int         *visited;

    lem_in->all_paths = malloc(sizeof(t_path) * lem_in->n_rooms);
    
    room = &lem_in->rooms[lem_in->start];
    if (!lem_in->all_paths) {
        perror("Failed to allocate memory for all_paths");
        exit(EXIT_FAILURE);
    }
    do {
        visited = reach_path(*lem_in, room);
        if (visited == NULL) {
            // printf("No path found from start to end.\n");
            break;
        }
        backtracking_path(lem_in, visited, &lem_in->all_paths[i]);
        i++;
    } while (visited != NULL);

    lem_in->n_paths = i;
        
    lem_in->all_paths[i].size = -1;

    int     *distribution = malloc(sizeof(int) * i);

    if (!distribution) {
        perror("Failed to allocate memory for distribution");
        free(lem_in->all_paths);
        exit(EXIT_FAILURE);
    }
    free(distribution);
    free(visited);
    compute_distribution(lem_in);
}

static void look_neighbors(t_lem_in lem_in, t_room *room, int *visited, t_queue *queue)
{
    int     links_size;
    int     room_index;

    links_size = 0;
    room_index = 0;
    
    if (room && room->links)
        links_size = get_links_size(room->links);
    
    for (int i = 0; i < links_size; i++)
    {
        room_index = room->links[i];
        
        if (room_index < 0 || room_index >= lem_in.n_rooms) {
            printf("Error: Invalid room index %d\n", room_index);
            continue;
        }

        t_room neighbors = lem_in.rooms[room_index];
        
        if (visited[room_index] == NOT_VISITED && !neighbors.ignored) {
            visited[room_index] = room->id;
            enqueue(queue, room_index);
        }
    }
}

static void init_visited(int *visited, int n_rooms)
{
    for (int i = 0; i < n_rooms; i++)
    {
        visited[i] = NOT_VISITED;
        // printf("visited[%d] = %d\n", i, visited[i]);
    }
}

static int *reach_path(t_lem_in lem_in, t_room *room)
{
    t_queue     *queue;
    int         *visited;
    int         current_node;
    int         target_node;

    visited = malloc(sizeof(int) * lem_in.n_rooms);
    if (!visited) {
        perror("Failed to allocate memory for visited array");
        exit(EXIT_FAILURE);
    }
    
    init_visited(visited, lem_in.n_rooms);
    visited[lem_in.start] = START;
    target_node = lem_in.end;
    queue = malloc(sizeof(t_queue));
    
    if (!queue) {
        perror("Failed to allocate memory for queue");
        free(visited);
        exit(EXIT_FAILURE);
    }
    init_queue(queue);
    enqueue(queue, room->id);
    
    while (!is_queue_empty(queue)) {
        int ok = dequeue(queue, &current_node);

        if (!ok)
            break;
        if (current_node == target_node)
            break;
        look_neighbors(lem_in, &lem_in.rooms[current_node], visited, queue);
    }

    if (visited[target_node] == NOT_VISITED) {
        free(visited);
        free(queue);
        return NULL;
    }

    destroy_queue(queue);
    free(queue);
    return visited;
}
