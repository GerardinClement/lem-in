#include "lem_in.h"

void init_ants(t_lem_in *lem_in)
{
    int i;
    int j;

    i = 0;
    j = 0;
    lem_in->ants = (t_ant *)malloc(sizeof(t_ant) * lem_in->n_ants);
    
    while (i < lem_in->n_ants)
    {
        lem_in->ants[i].id = i + 1;
        lem_in->ants[i].room = &lem_in->rooms[lem_in->start];
        lem_in->ants[i].current_path_pos = lem_in->all_paths[j].size - 1;
        lem_in->ants[i].path_id = j;

        lem_in->all_paths[j].distribution--;
        j++;
        
        if (lem_in->all_paths[j].distribution == 0)
            j = 0;

        i++;
    }
}