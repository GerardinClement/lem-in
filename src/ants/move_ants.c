#include "lem_in.h"

static void     move_ants(t_lem_in *lem_in);
static void     write_ant_move(int ant_id, char *room_name);
static t_room   *get_next_room(t_lem_in *lem_in, t_room *room);
static t_room   **get_most_valuable_room(t_lem_in *lem_in, int *links);
static void     sort_by_cost(t_room **rooms, size_t size);
// static void     display_all_links(t_room *room, t_room **rooms_linked);

void    move_ants_manager(t_lem_in *lem_in)
{
    move_ants(lem_in);
}

static void    move_ants(t_lem_in *lem_in)
{
    t_room      *next_room;
    t_ant       *ants;

    ants = lem_in->ants;
    for (int i = 0; i < lem_in->n_ants; i++)
    {
        if (ants[i].room->id == lem_in->end)
            continue;
        // printf("Ant %d is in room %s\n", ants[i].id, ants[i].room->name);
        next_room = get_next_room(lem_in, ants[i].room);
        if (next_room->is_empty == false && next_room->id != lem_in->end)
        {
            // printf("Ant %d is blocked in room %s\n", ants[i].id, ants[i].room->name);
            continue;
        }
        if (next_room->id == ants[i].room->id)
            continue;
        ants[i].room->is_empty = true;
        next_room->is_empty = false;
        ants[i].room = next_room;
        write_ant_move(ants[i].id, next_room->name);
    }
    ft_printf("\n");
}


static t_room   *get_next_room(t_lem_in *lem_in, t_room *room)
{
    t_room      *next_room;
    t_room      **rooms_linked;
    size_t      i;
    size_t      links_len;

    i = 0;
    if (!room->links) {
        return (room);
    }
    links_len = get_links_size(room->links);
    rooms_linked = get_most_valuable_room(lem_in, room->links);
    // display_all_links(room, rooms_linked);
    while (i < links_len && i < 3)
    {
        next_room = rooms_linked[i];
        if ((next_room->is_empty == true && next_room->cost != -1) || next_room->id == lem_in->end)
        {
            free(rooms_linked);
            return (next_room);
        }
        i++;
    }
    free(rooms_linked);
    return (room);
}

static t_room **get_most_valuable_room(t_lem_in *lem_in, int *links)
{
    t_room      **best_rooms;
    int         links_len;
    size_t      i;

    i = 0;
    links_len = get_links_size(links);
    best_rooms = malloc(sizeof(t_room *) * links_len);
    while (links[i] != -1)
    {
        best_rooms[i] = &lem_in->rooms[links[i]];
        i++;
    }
    sort_by_cost(best_rooms, links_len);
    return (best_rooms);
}

static void   sort_by_cost(t_room **rooms, size_t size)
{
    t_room      *tmp;
    size_t      i;
    size_t      j;

    i = 0;
    while (i < size)
    {
        j = 0;
        while (j < size - 1)
        {
            if (rooms[j]->cost < rooms[j + 1]->cost)
            {
                tmp = rooms[j];
                rooms[j] = rooms[j + 1];
                rooms[j + 1] = tmp;
            }
            j++;
        }
        i++;
    }
}

static void write_ant_move(int ant_id, char *room_name)
{
    ft_printf("L%d-%s ", ant_id, room_name);
}