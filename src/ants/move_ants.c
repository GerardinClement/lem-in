#include "lem_in.h"

static void     move_ants(t_lem_in *lem_in);
static void     write_ant_move(int ant_id, char *room_name);
static t_room   *get_next_room(t_lem_in *lem_in, t_ant *ant);


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
        next_room = get_next_room(lem_in, &ants[i]);
        if (next_room->is_empty == false && next_room->id != lem_in->end) {
            // if (next_room->is_empty == false)
            //     printf("Error: Room %s is not empty, cannot move ant %d\n", next_room->name, ants[i].id);
            continue;
        }
        if (next_room->id == ants[i].room->id)
            continue;
        ants[i].room->is_empty = true;
        next_room->is_empty = false;
        ants[i].room = next_room;
        ants[i].current_path_pos--;
        write_ant_move(ants[i].id, next_room->name);
    }
    ft_printf("\n");
}


static t_room   *get_next_room(t_lem_in *lem_in, t_ant *ant)
{
    t_path      path = lem_in->all_paths[ant->path_id];
    int         next_room_id = 0;
    
    if ((size_t)ant->current_path_pos != path.size) 
        next_room_id = path.path[ant->current_path_pos];
    else
        next_room_id = path.path[ant->current_path_pos - 1];

    t_room      *next_room = &lem_in->rooms[next_room_id];

    return next_room;
}


static void write_ant_move(int ant_id, char *room_name)
{
    (void)room_name; // To avoid unused parameter warning
    (void)ant_id; // To avoid unused parameter warning
    ft_printf("L%d-%s ", ant_id, room_name);
}