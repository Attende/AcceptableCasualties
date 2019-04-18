#include "enemy.h"

const int maxRight = 4000;
const int maxUp = 2000;
enemy::enemy() : object() { }
enemy::enemy(glm::vec2 _pos, glm::vec2 _size, glm::vec2 _vel, Texture _sprite)
	: object(_pos, _size, _sprite, _vel, glm::vec3(1.0f), 0.0f)
{
	Horizontal = rightEn;
}

void enemy::jump(float _vel)
{
	vel.y += _vel;
}


glm::vec2 enemy::move(float dt, int left = 0, int right = 800, int top = 0, int bottom = 600)
{
	if (type == 0)
	{
		xMove(dt, left, right);
	}
	if (type == 1)
	{
		xMove(dt, left, right);
		yMove(dt, top, bottom);
	}
	return pos;

}glm::vec2 enemy::deltav(float dt, int direction, float accelMult)
{
	if (direction == 1) //Positive Horizontal
	{
		vel += glm::vec2(acceleration.x * dt * accelMult, 0);
	}
	if (direction == 2) //Negative Horizontal
	{
		vel -= glm::vec2(acceleration.x * dt* accelMult, 0);
	}
	if (direction == 3) //Positive Vertical
	{
		vel += glm::vec2(0, acceleration.y * dt* accelMult);
	}
	if (direction == 4) //Negative Vertical
	{
		vel -= glm::vec2(0, acceleration.y * dt* accelMult);
	}
	vel = vel * coeffFriction;
	return vel;

}
float enemy::xMove(float dt, int left, int right)
{
	pos.x += vel.x * dt;
	if (pos.x <= left)
	{
		vel.x = -vel.x;
		pos.x = left;
	}
	if (pos.x >= right - size.x)
	{
		vel.x = -vel.x;
		pos.x = right - size.x;
	}
	return pos.x;
}

float enemy::xMove2(float dt)
{
	if (Horizontal == leftEn)
	{
		pos.x -= vel.x * dt;
	}
	else if (Horizontal == rightEn)
	{
		pos.x += vel.x * dt;

	}
	return pos.x;
}
float enemy::yMove(float dt, int top, int bottom)
{
	pos.y += vel.y * dt;

	if (pos.y <= top)
	{
		vel.y = 0;
		pos.y = top;
	}
	if (pos.y >= bottom - size.y)
	{
		vel.y = 0;
		pos.y = bottom - size.y;
		destroyed = true;
		doCollisions = false;
	}
	return pos.y;
}

void enemy::restart(glm::vec2 _pos, glm::vec2 _vel)
{
	pos = _pos;
	vel = _vel;
}