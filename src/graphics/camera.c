#include "lem_in.h"

t_camera camera;
float rotation_angle = 0.0f;
float vertical_angle = 0.0f;

float *get_rotation_angle() 
{
	return &rotation_angle;
}

t_camera *get_camera() 
{
	return &camera;
}

void move_camera(t_vector3 direction) 
{
	camera.eye.x += direction.x;
	camera.eye.y += direction.y;
	camera.eye.z += direction.z;
    camera.center.x += direction.x;
    camera.center.y += direction.y;
    camera.center.z += direction.z;
}

void rotate_camera(int x, int y) 
{
	float radius = 20.0f;
	rotation_angle += x * 0.01f;
    vertical_angle += y * 0.01f;
	if (rotation_angle > 2 * M_PI) {
		rotation_angle -= 2 * M_PI;
	} else if (rotation_angle < 0) {
		rotation_angle += 2 * M_PI;
	}

    if (vertical_angle > 2 * M_PI) {
        vertical_angle -= 2 * M_PI;
    }

	camera.eye.x = camera.center.x + radius * cos(rotation_angle);
    camera.eye.y = camera.center.y + radius * sin(vertical_angle);
	camera.eye.z = camera.center.z + radius * sin(rotation_angle);
}

void setup_camera(t_sphere *points, int size) 
{
    t_vector3 centroid;
    t_vector3 eye;
    t_vector3 up;

    centroid.x = 0;
    centroid.y = 0;
    centroid.z = 0;

    for (int i = 0; i < size; i++) 
	{
        centroid.x += points[i].pos.x;
        centroid.y += points[i].pos.y;
        centroid.z += points[i].pos.z;
    }
    centroid.x /= size;
    centroid.y /= size;
    centroid.z /= size;

    camera.radius = 20.0f;
    eye.x = centroid.x + camera.radius * cos(rotation_angle);
    eye.y = centroid.y + camera.radius * sin(vertical_angle);
    eye.z = centroid.z + camera.radius * sin(rotation_angle);

    up.x = 0;
    up.y = 1;
    up.z = 0;

    camera.eye = eye;
    camera.center = centroid;
    camera.up = up;

    camera.locked_on_ant = false;
}

void lock_camera_on_ant() 
{
    if (camera.locked_on_ant)
        camera.locked_on_ant = false;
    else
        camera.locked_on_ant = true;  
}

void follow_ant() 
{
    t_sphere *ants = get_ants();
    t_vector3 pos = ants[camera.ant_followed].pos;
    camera.center.x = pos.x;
    camera.center.y = pos.y;
    camera.center.z = pos.z;
    camera.eye.x = pos.x + camera.radius * cos(rotation_angle);
    camera.eye.y = pos.y + camera.radius * sin(vertical_angle);
    camera.eye.z = pos.z + camera.radius * sin(rotation_angle);
}
