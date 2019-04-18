#include "camera2D.h"




camera2d::camera2d(vec2 _pos = vec2(0.0f, 0.0f), float _rot = ROT, float _zoom = ZOOM)
{
	pos = _pos;
	rot = _rot;
	zoom = _zoom;
	moveSpeed = SPEED;
	yOffset = 5.0f;
	xOffset = 5.0f;
}

void camera2d::keyboard(int direction, float dt)
{
	glm::vec2 vel = moveSpeed * dt;
	//Movement
	if (direction == 1) //Up
	{
		pos.y -= vel.y;
	}
	if (direction == 2) //Down
	{
		pos.y += vel.y;
	}
	if (direction == 3) //Left
	{
		pos.x -= vel.x;
	}
	if (direction == 4) //Right
	{
		pos.x += vel.x;
	}
	if (direction == 5) // Rotate Anticlockwise
	{
		rot -= ROTSPEED * dt;
	}
	if (direction == 6) // Rotate Clockwise
	{
		rot += ROTSPEED * dt;
	}
	if (direction == 7) // Zoom in
	{
		zoom += ZOOMSPEED * dt;
	}
	if (direction == 8) // Zoom out
	{
		zoom -= ZOOMSPEED * dt;
	}
	if (zoom > ZOOMMAX)
	{
		zoom = ZOOMMAX;
	}
	if (zoom < ZOOMMIN)
	{
		zoom = ZOOMMIN;
	}
	update();
}

mat4 camera2d::getViewMatrix()
{
	mat4 view = mat4(1);
	vec2 midOffset = vec2(width * 0.5, height * 0.5);
	//Scale
	//Rotate
	view = translate(view, vec3(midOffset, 0.0f));
	view = scale(view, vec3(zoom, zoom, 1.0f));
	view = rotate(view, -rot, vec3(0.0f, 0.0f, 1.0f));
	view = translate(view, vec3(-midOffset, 0.0f));
	//Translate
	view = translate(view, vec3(pos, 0.0f));
	return view;
}
void camera2d::setPos(vec2 _pos)
{
	pos = _pos;
}

void camera2d::update()
{

}