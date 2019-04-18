#pragma once
#include "object.h"

class enemy : public object
{
public:
	enum hor {
		leftEn,
		rightEn
	};
	hor Horizontal;
	bool collidedLastFrame = false;
	float coeffFriction = 0.999f;
	int type;
	glm::vec2 initialPos;
	enemy();
	enemy(glm::vec2 pos, glm::vec2 size, glm::vec2 vel, Texture sprite);
	glm::vec2 deltav(float dt, int direction, float mult);
	glm::vec2 move(float dt, int leftBound, int rightBound, int upBound, int downBound);
	float xMove2(float dt);
	float xMove(float dt, int leftBound, int rightBound);
	float yMove(float dt, int upBound, int downBound);
	void jump(float vel);
	void restart(glm::vec2 pos, glm::vec2 vel);
private:

};