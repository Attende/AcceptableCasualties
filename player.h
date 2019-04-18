#pragma once
#include "object.h"


class player : public object
{
public:
	bool canTakeDamage;
	int score = 0;
	int lives = 3;
	int hp = 2;
	bool dead = false;
	bool collidedLastFrame = false;
	float coeffFriction = 0.999f;
	player();
	player(glm::vec2 pos, glm::vec2 size, glm::vec2 vel, Texture sprite);
	glm::vec2 deltav(float dt, int direction, float mult);
	glm::vec2 move(float dt, int leftBound, int rightBound, int upBound, int downBound);
	float xMove(float dt, int leftBound, int rightBound);
	float yMove(float dt, int upBound, int downBound);
	void jump(float vel);
	void restart(glm::vec2 pos, glm::vec2 vel);

private:
	
};