#include "lem_in.h"

static bool     init_lem_in(t_lem_in *lem_in, char **data);

t_lem_in parse_data(t_array *data)
{  
    t_lem_in    lem_in;

    if (!init_lem_in(&lem_in, data->arr))
    {
        printf("Init lemin error\n");
        exit(1);
    }

    parse_nb_ants(&lem_in, data);
    parse_rooms(&lem_in, data);
    parse_links(&lem_in, data);
    free_array(data);
    free(data);
    return (lem_in);
}

static bool init_lem_in(t_lem_in *lem_in, char **data)
{
    lem_in->ants = 0;

    lem_in->start = -1;
    lem_in->end = -1;
    lem_in->n_rooms = 0;

    lem_in->rooms = malloc(sizeof(t_room) * (count_n_rooms(data) + 1));
    if (!lem_in->rooms)
    {
        return (false);
    }
    
    return (true);
}