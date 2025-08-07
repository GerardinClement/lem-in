#ifndef LEM_IN_H
# define LEM_IN_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <limits.h>
# include <stdbool.h>
# include <time.h>

# include "../libft/include/libft.h"
# include "struct.h"
# include "link.h"
# include "room.h"
# include "parsing.h"
# include "errors.h"
# include "free.h"
# include "graphics.h"
# include "algo.h"
# include "queue.h"
# include "edge.h"

# define NUM_POINTS 100
# include "ants.h"

// ----------------------------- Global utils -----------------------------

void        *ft_realloc(void *ptr, size_t size, size_t len);

// ----------------------------- Display functions -----------------------------

void        display_data(t_lem_in *lem_in);
void        display_info_rooms_links(t_lem_in *lem_in);
void        display_info_rooms_status(t_lem_in *lem_in);
void        display_info_ants(t_lem_in *lem_in);

int         generate_random_number(int min, int max);


#endif