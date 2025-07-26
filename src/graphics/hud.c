#include "lem_in.h"

void render_text(float x, float y, const char *text) 
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Déplacement à la position de texte
    glRasterPos2f(x, y);

    // Rendre chaque caractère
    for (const char *c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    
    // Rétablissement des matrices d'origine
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void display_room_info(t_room room) 
{
	char buffer[50];
	sprintf(buffer, "Salle %s Cost: %d", room.name, room.cost);
	render_text(WIDTH - (WIDTH / 8), HEIGHT - (HEIGHT / 10), buffer);
}

void display_ant_info(t_ant ant) 
{
	char buffer[50];
	sprintf(buffer, "Fourmi %d dans la salle %s", ant.id, ant.room->name);
	render_text(WIDTH - (WIDTH / 8), HEIGHT - (HEIGHT / 20), buffer);
	display_room_info(*ant.room);
}

void draw_hud(t_lem_in lem_in, int iterations) 
{
	t_camera *camera = get_camera();
	char buffer[50];
	sprintf(buffer, "Nombre d'iterations: %d", iterations + 1);
	render_text(10,  HEIGHT - (HEIGHT / 20), buffer);
	if (camera->locked_on_ant) {
		display_ant_info(lem_in.ants[camera->ant_followed]);
	}
}

