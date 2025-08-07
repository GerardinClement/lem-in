#ifndef GRAPHICS_H
#define GRAPHICS_H

# include <GL/glew.h>
# include <GL/glut.h> 
# include<math.h> 

#include "lem_in.h"

#define WIDTH 1920
#define HEIGHT 1080

typedef struct s_camera {
	t_vector3 	center;
	t_vector3 	eye;
	t_vector3 	up;
	bool		locked_on_ant;
	int			ant_followed;
	float		radius;
}				t_camera;

typedef struct color {
	float 		r;
	float 		g;
	float 		b;
	float 		o;
}				t_color;


typedef struct s_sphere {
	t_vector3	pos;
	t_vector3	*links;
	size_t		links_size;
	t_color		color;
}				t_sphere;


void 		init_window(int argc, char **argv, t_lem_in lem_in);
void 		special_keyboard_listener(int key, int x, int y);
t_sphere	*get_ants();
t_sphere 	*get_rooms();
int 		get_n_ants();
int 		get_n_rooms();

void 		setup_camera(t_sphere *points, int size);
float 		*get_rotation_angle();
t_camera 	*get_camera();
void 		move_camera(t_vector3 direction);
void 		rotate_camera(int x, int y);
void 		lock_camera_on_ant();
void 		follow_ant();
void 		render_text(float x, float y, const char *text);
void 		draw_hud(t_lem_in lem_in, int iterations); 

void 		mouse_listener(int button, int state, int x, int y);
void 		mouse_motion_listener(int x, int y);

void 		set_paused();

#endif