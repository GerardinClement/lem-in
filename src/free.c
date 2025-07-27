#include "lem_in.h"

void free_room(t_room *room)
{
	free(room->links);
	free(room->name);
}

void free_lem_in(t_lem_in *lem_in)
{
	int i;

	i = 0;
	while (i < lem_in->n_rooms)
	{
		free_room(&lem_in->rooms[i]);
		i++;
	}
	free(lem_in->rooms);
	free(lem_in->ants);
	free(lem_in->all_paths);
}

void free_array(t_array *data)
{
	size_t i;

	i = 0;
	while (i < data->size)
	{
		free(data->arr[i]);
		i++;
	}
	free(data->arr);
}
