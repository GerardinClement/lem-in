#include "lem_in.h"

static bool is_duplicate_link(int *links,  int link)
{
	size_t		i;

	i = 0;
	if (!links)
		return (false);

	while (links[i] != -1)
	{
		if (links[i] == link)
			return (true);
		i++;
	}

	return (false);
}

static void copy_links(int *links, int *new_links)
{
	size_t i;

	i = 0;
	if (!links)
		return ;
	while (links && links[i] != -1)
	{
		new_links[i] = links[i];
		i++;
	}
}

static bool add_link(int **links, t_room *linked_room)
{
    size_t size;

    if (is_duplicate_link(*links, linked_room->id))
    {
        print_error("Duplicate link.\n");
        return (false);
    }

    size = get_links_size(*links);
    int *tmp = (int *)ft_realloc(*links, sizeof(int), size + 2);
    if (!tmp)
    {
        print_error(ERR_MALLOC);
        return (false);
    }
    tmp[size + 1] = -1;

    copy_links(*links, tmp);
    free(*links);
    *links = tmp;
    (*links)[size] = linked_room->id;
    return (true);
}

int get_links_size(int *links)
{
	int i;

	i = 0;
    // printf("get_links_size: %p\n", links);

	while (links && links[i] != -1)
		i++;
	return (i);
}

bool set_link_in_rooms(t_lem_in *lem_in, char *room1, char *room2, t_array *data)
{
    t_room *room;
    t_room *linked_room;

    room = get_room(lem_in, room1);
    linked_room = get_room(lem_in, room2);

    if (!room || !linked_room)
        fatal_errors_handler(lem_in, "Room does not exist.\n", data);

    if (!add_link(&(room->links), linked_room))
        return (false);
    return (true);
}