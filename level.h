#pragma once
#include "object.h"
#include "spriteRenderer.h"
#include <vector>
#include "glad/glad.h"
#include <glm/glm.hpp>
#include "resourceManager.h"
#include "enemy.h"

class level
{
public:
	std::vector<object> walls;
	std::vector<object> checkpoints;
	std::vector<enemy> enemies; // Enemy class to be developed past constructor
	std::vector<object> flags;
	std::vector<object> coins;
	level();
	int xMax, yMax;
	void init(std::vector<std::vector<unsigned int>> tileData, Texture texture, unsigned int Width, unsigned int Height);
	void draw(spriteRenderer &renderer);
	bool finished;

private:

};