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

static void backtracking_path(t_lem_in *lem_in ,int *visited) {
    int prev_node = visited[lem_in->end];
    int i = 0;

    lem_in->rooms[prev_node].ignored = 1; // Mark the end room as ignored
    while (prev_node != START) {
        if (prev_node == NOT_VISITED) {
            print_error("No path found from start to end.\n");
            return;
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
    printf("path size: %d\n", i + 1);
}

static void calculate_all_rooms_cost(t_lem_in *lem_in) 
{
    t_room *room;
    int **all_paths;
    int i = 0;

    room = &lem_in->rooms[lem_in->start];
    all_paths = malloc(sizeof(int *) * lem_in->n_rooms);
    int *visited = reach_path(*lem_in, room);

    while (visited != NULL) {
        visited = reach_path(*lem_in, room);
        backtracking_path(lem_in, visited);
        printf("-----------------------------------------------------------\n");
        all_paths[i] = visited;
        i++;
    }

    for (int j = 0; j < i; j++)
    {
        if (all_paths[j] != NULL)
            backtracking_path(lem_in, all_paths[j]);
        else
            printf("No path found for room %d\n", j);
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
    init_visited(visited, lem_in.n_rooms);
    visited[lem_in.start] = START; // Mark the start room as visited
    target_node = lem_in.end;
    queue = malloc(sizeof(t_queue));

    init_queue(queue);
    enqueue(queue, room->id);
    while (!is_queue_empty(queue)) {
        current_node = dequeue(queue);
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
