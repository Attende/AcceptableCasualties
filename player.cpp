#include "player.h"
const int maxRight = 4000;
const int maxUp = 2000;
player::player() : object() { }
player::player(glm::vec2 _pos, glm::vec2 _size, glm::vec2 _vel, Texture _sprite)
	: object(_pos, _size, _sprite, _vel, glm::vec3(1.0f),0.0f) {}

void player::jump(float _vel)
{
	vel.y += _vel;
	std::cout << "Jumping" << std::endl;
}


glm::vec2 player::move(float dt, int left = 0, int right = 800, int top = 0, int bottom = 600)
{
	xMove(dt, 0, maxRight);
	yMove(dt, 0, maxUp);
	return pos;

}


glm::vec2 player::deltav(float dt, int direction, float accelMult)
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

float player::xMove(float dt, int left, int right)
{
	pos.x += vel.x * dt;
	if (pos.x <= left)
	{
		vel.x = 0;
		pos.x = left;
	}
	if (pos.x >= right - size.x)
	{
		vel.x = 0;
		pos.x = right - size.x;
	}
	return pos.x;
}

float player::yMove(float dt, int top, int bottom)
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
		dead = true;
	}
	return pos.y;
}

void player::restart(glm::vec2 _pos, glm::vec2 _vel)
{
	dead = false;
	pos = _pos;
	vel = _vel;
}

 