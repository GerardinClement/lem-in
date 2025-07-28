#include "lem_in.h"

void init_ants(t_lem_in *lem_in)
{
    int         i;
    int         j;
    int         total_paths;

    i = 0;
    j = 0;
    lem_in->ants = (t_ant *)malloc(sizeof(t_ant) * lem_in->n_ants);

    if (!lem_in->all_paths || !lem_in->ants)
    {
        perror("Failed to allocate memory for ants or paths");   
        return;
    }

    total_paths = lem_in->n_paths;
    
    while (i < lem_in->n_ants) {
        lem_in->ants[i].id = i + 1;
        lem_in->ants[i].room = &lem_in->rooms[lem_in->start];
        
        if (j >= total_paths)
            j = 0;
        
        lem_in->ants[i].current_path_pos = lem_in->all_paths[j].size - 1;
        lem_in->ants[i].path_id = j;
        
        lem_in->all_paths[j].distribution--;
        j++;
        
        if (j >= total_paths || lem_in->all_paths[j].distribution == 0)
            j = 0;
        
        i++;
    }
}