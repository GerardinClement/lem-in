#include "lem_in.h"

static bool     check_extension(char *filename);


static bool check_extension(char *filename)
{
    int i;

    i = 0;
    while (filename[i])
        i++;
    if (filename[i - 1] != 'p' 
        || filename[i - 2] != 'a' 
        || filename[i - 3] != 'm' 
        || filename[i - 4] != '.')
    {
        print_error("Please use a .map file.\n");
        return (false);
    }
    return (true);
}

bool check_if_all_ants_in_end2(t_lem_in *lem_in) {
    for (int i = 0; i < lem_in->n_ants; i++)
    {
        if (lem_in->ants[i].room->id != lem_in->end)
            return false;
    }
    return true;
}




int main(int argc, char **argv)
{
    t_array *data;
    t_lem_in lem_in;

    int win_mode = 0;
    char *map_file = NULL;

    (void)win_mode;

    if (argc < 2 || argc > 3) {
        print_error("Usage: ./lem-in [file] [--win]\n");
        return (EXIT_FAILURE);
    }

    if (argc == 3) {
        if (ft_strncmp(argv[2], "--win", 5) == 0)
            win_mode = 1;
        else {
            print_error("Usage: ./lem-in [file] [--win]\n--win: execute with graphical window.\n");
            return (EXIT_FAILURE);
        }
    }
    map_file = argv[1];

    if (!check_extension(map_file))
        return (EXIT_FAILURE);

    srand(time(NULL));
    data = get_data(map_file);
    if (!data)
        return (EXIT_FAILURE);
    lem_in = parse_data(data);

    algo_manager(&lem_in);
    init_ants(&lem_in);


    if (win_mode) {
        init_window(argc, argv, lem_in);
    } else {
        while (!check_if_all_ants_in_end2(&lem_in))
            move_ants_manager(&lem_in);
    }

    // free_lem_in(&lem_in);
    return (EXIT_SUCCESS);
}