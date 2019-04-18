#pragma once
#include "Texture.h"
#include "spriteRenderer.h"

class object
{
public:
	glm::vec2 pos, size, vel;
	glm::vec3 colour;
	float rot;
	bool doCollisions, destroyed;
	Texture sprite;
	//glm::vec2 maxVel = glm::vec2(400.0f);
	glm::vec2 acceleration = glm::vec2(400.0f,800.0f);
	object();
	object(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec2 velocity, glm::vec3 colour, float rot);
	virtual void draw(spriteRenderer &spriteRenderer);
	glm::mat4 getModelMatrix(spriteRenderer &renderer);
private:

};







