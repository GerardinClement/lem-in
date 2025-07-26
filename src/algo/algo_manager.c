#include "lem_in.h"

static void calculate_all_rooms_cost(t_lem_in *lem_in);
static int *reach_path(t_lem_in lem_in, t_room *room);
// static void print_all_rooms_cost(t_lem_in *lem_in);


const int NOT_VISITED = -1;
const int START = -2;

void    algo_manager(t_lem_in *lem_in)
{
    calculate_all_rooms_cost(lem_in);
}

#include <stdio.h>

static int compute_distribution(t_lem_in *lem_in) {
    int T;
    int num_ants = lem_in->n_ants;

    // 1. Trouver la longueur minimale
    T = lem_in->all_paths_size[0];
    int nb_paths = 0;
    for (int i = 1; lem_in->all_paths_size[i] != -1; i++) {
        if (lem_in->all_paths_size[i] < T)
            T = lem_in->all_paths_size[i];
        nb_paths++;
    }

    int capacities[nb_paths];

    // 2. Boucle pour trouver le T optimal
    while (1) {
        int total_capacity = 0;

        for (int i = 0; i < nb_paths; i++) {
            capacities[i] = T - lem_in->all_paths_size[i] + 1;
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


static int backtracking_path(t_lem_in *lem_in ,int *visited) {
    int prev_node = visited[lem_in->end];
    int i = 0;

    lem_in->rooms[prev_node].ignored = 1; // Mark the end room as ignored
    while (prev_node != START) {
        if (prev_node == NOT_VISITED) {
            print_error("No path found from start to end.\n");
            return -1; // No path found
        }
        int current_id = lem_in->rooms[prev_node].id;
        prev_node = visited[current_id];
        i++;
    }
    int *path = malloc(sizeof(int) * (i + 1));
    prev_node = visited[lem_in->end];
    for (int j = 0; j <= i; j++) {
        path[j] = lem_in->rooms[prev_node].id;
        prev_node = visited[path[j]];
    }
    printf("Start: %d\n", lem_in->start);
    printf("End: %d\n", lem_in->end);
    for (int j = i; j >= 0; j--) {
        printf("Room %d\n", path[j]);
    }
    printf("path size: %d\n", i);
    return i;
}

static void calculate_all_rooms_cost(t_lem_in *lem_in) 
{
    t_room *room;
    int *visited;
    lem_in->all_paths = malloc(sizeof(t_path) * lem_in->n_rooms);
    lem_in->all_paths_size = malloc(sizeof(int) * lem_in->n_rooms);

    if (!lem_in->all_paths_size) {
        perror("Failed to allocate memory for all_paths_size");
        exit(EXIT_FAILURE);
    }
    int i = 0;

    printf("DEBUG: Starting path search from start room id %p\n", lem_in->rooms);
    printf("DEBUG: Number of rooms: %d\n", lem_in->n_rooms);
    room = &lem_in->rooms[lem_in->start];
    printf("DEBUG: Starting path search from room id %d\n", room->id);
    if (!lem_in->all_paths) {
        perror("Failed to allocate memory for all_paths");
        free(lem_in->all_paths_size);
        exit(EXIT_FAILURE);
    }
    do {
        printf("DEBUG: Starting path search from room id %d\n", room->id);
        visited = reach_path(*lem_in, room);
        if (visited == NULL) {
            printf("No path found from start to end.\n");
            break;
        }
        lem_in->all_paths[i].size = backtracking_path(lem_in, visited);
        printf("-----------------------------------------------------------\n");
        lem_in->all_paths[i].path = visited;
        i++;
    } while (visited != NULL && i < 5);
    lem_in->all_paths[i].size = -1;
    int *distribution = malloc(sizeof(int) * i);
    if (!distribution) {
        perror("Failed to allocate memory for distribution");
        free(lem_in->all_paths);
        free(lem_in->all_paths_size);
        exit(EXIT_FAILURE);
    }
    int T = compute_distribution(lem_in);
    printf("T = %d\n", T);
    for (int j = 0; j < i; j++) {
        printf("Path %d: size = %d, ants = %d\n", j, lem_in->all_paths_size[j], lem_in->all_paths[j].distribution);
    }

}

static void look_neighbors(t_lem_in lem_in, t_room *room, int *visited, t_queue *queue)
{
    int i;
    int links_size;

    links_size = get_links_size(room->links);
    for (i = 0; i < links_size; i++)
    {
        t_room neighbors = lem_in.rooms[room->links[i]];
        if (visited[room->links[i]] == NOT_VISITED && !neighbors.ignored) {
            visited[room->links[i]] = room->id;
            enqueue(queue, room->links[i]);
        }
    }
}

static void init_visited(int *visited, int n_rooms)
{
    for (int i = 0; i < n_rooms; i++)
        visited[i] = NOT_VISITED;
}

static int *reach_path(t_lem_in lem_in, t_room *room)
{
    t_queue *queue;
    int *visited = malloc(sizeof(int) * lem_in.n_rooms);
    int current_node;
    int target_node;

    if (!visited) {
        perror("Failed to allocate memory for visited array");
        exit(EXIT_FAILURE);
    }
    printf("DEBUG: reach_path called for room id %d\n", room->id);
    init_visited(visited, lem_in.n_rooms);
    visited[lem_in.start] = START; // Mark the start room as visited
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
        return NULL; // No path found
    }
    return visited;
}

// static void print_all_rooms_cost(t_lem_in *lem_in)
// {
//     for (int i = 0; i < lem_in->n_rooms; i++)
//     {
//         if (lem_in->rooms[i].id == lem_in->start)
//             printf("Room %d (start) cost: %d\n", lem_in->rooms[i].id, lem_in->rooms[i].cost);
//         else if (lem_in->rooms[i].id == lem_in->end)
//             printf("Room %d (end) cost: %d\n", lem_in->rooms[i].id, lem_in->rooms[i].cost);
//         else
//             printf("Room %d cost: %d\n", lem_in->rooms[i].id, lem_in->rooms[i].cost);
//     }
// }
