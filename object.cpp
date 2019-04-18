#include "object.h"

object::object()
{
	pos = glm::vec2(0);
	size = glm::vec2(1);
	vel = glm::vec2(0);
	rot = 0.0f;
	colour = glm::vec3(0.0f);
	doCollisions = false;
	destroyed = false;		
	sprite;
}

object::object(glm::vec2 _pos, glm::vec2 _size, Texture _sprite, glm::vec2 _vel, glm::vec3 _colour, float _rot)
{
	pos = _pos;
	size = _size;
	vel = _vel;
	rot = _rot;
	colour = _colour;
	doCollisions = false;
	destroyed = false;
	sprite = _sprite;
}

void object::draw(spriteRenderer &renderer)
{
	renderer.drawSprite(sprite, pos, size, rot, colour);
}
glm::mat4 object::getModelMatrix(spriteRenderer &renderer)
{
	glm::mat4 modelMat = renderer.tempTest;
	return modelMat;
}