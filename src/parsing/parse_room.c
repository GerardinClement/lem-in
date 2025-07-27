#include "lem_in.h"

static t_vector3 get_room_pos(char *line)
{
    t_vector3       pos;
    char            **split;
    int             x;
    int             y;

    split = ft_split(line, ' ');
    if (!split)
    {
        print_error(ERR_SPLIT);
        return ((t_vector3){-1, -1, -1});
    }
    if (ft_array_len(split) != 3 || !is_num(split[1]) || !is_num(split[2]))
    {
        ft_free_array(split);
        return ((t_vector3){-1, -1, -1});
    }

    x = ft_atoi(split[1]);
    y = ft_atoi(split[2]);
    pos.x = x + generate_random_number(0, x / 2);
    pos.y = y + generate_random_number(0, y / 2);
    pos.z = generate_random_number(0, pos.x + pos.y);

    ft_free_array(split);
    return (pos);
}

static char *get_room_name(char *line)
{
    char *name;
    int i;

    i = 0;
    while (line[i] != ' ')
        i++;
    name = ft_substr(line, 0, i);
    if (!name)
    {
        print_error(ERR_MALLOC);
        return (NULL);
    }

    return (name);
}

void set_command(t_lem_in *lem_in, char *line, int room_id)
{
    if (ft_strncmp(line, "##start", ft_strlen(line)) == 0)
    {
        lem_in->start = room_id;
        lem_in->rooms[room_id].is_empty = false;
    }
    else if (ft_strncmp(line, "##end", ft_strlen(line)) == 0)
        lem_in->end = room_id;
}

bool check_if_room(char *line)
{
    if (check_if_link(line) || check_if_command(line))
        return (false);
    if (line[0] == 'L')
        return (false);
    if (strchr(line, ' ') == NULL)
        return (false);
    return (true);
}

int count_n_rooms(char **data)
{
    int i;
    int n_rooms;

    i = 0;
    n_rooms = 0;
    while (data && data[i])
    {
        if (check_if_room(data[i]))
            n_rooms++;
        i++;
    }
    return (n_rooms);
}

void parse_rooms(t_lem_in *lem_in, t_array *data)
{
    int *i;
    int id;

    i = &data->index;
    id = 0;
    while (data->arr[*i] && (check_if_room(data->arr[*i]) || check_if_command(data->arr[*i])))
    {
        if (check_if_command(data->arr[*i]))
        {
            *i += 1;
            continue;
        }
        t_vector3 room_pos = get_room_pos(data->arr[*i]);
        if (room_pos.x < 0 || room_pos.y < 0)
            fatal_errors_handler(lem_in, "Wrong room format.\n", data);
        char *room_name = get_room_name(data->arr[*i]);
        if (check_if_room_exist(lem_in, room_name))
        {
            printf("Room name: %s\n", room_name);
            fatal_errors_handler(lem_in, "Room already exist.\n", data);
        }
        set_room(&lem_in->rooms[id], id, room_name, room_pos);
        if (check_if_command(data->arr[*i - 1]))
            set_command(lem_in, data->arr[*i - 1], id);
        id++;
        *i += 1;
        lem_in->n_rooms++;
    }
    if (lem_in->n_rooms == 0)
        fatal_errors_handler(lem_in, "No rooms found.\n", data);
    if (lem_in->start == -1 || lem_in->end == -1)
        fatal_errors_handler(lem_in, "No start or end room found.\n", data);
    lem_in->rooms[id].id = -1;
}
