#include "level.h"
const int noOfRows = 6;
const int noOfColumns = 8;	//Maintain to Aspect Ratio

level::level()
{

}

void level::init(std::vector<std::vector<unsigned int>> tileData, Texture texture, unsigned int ScreenWidth, unsigned int ScreenHeight)
{
	unsigned int width = tileData[0].size();
	//std::cout << width << std::endl;
	unsigned int height = tileData.size();
	//std::cout << height << std::endl;
	float blockWidth = ScreenWidth / noOfColumns;
	float blockHeight = ScreenHeight / noOfRows;
	xMax = blockWidth * width;
	yMax = blockHeight * height;
	for (int y = 0; y < height; y++)
	{

		for (int x = 0; x < width; ++x) //Pre increment because of how coordinates are stored.
		{
			glm::vec2 pos(blockWidth * x, blockWidth * y);
			glm::vec2 size(blockWidth, blockHeight);
			if (tileData[y][x] == 1)
			{
				object obj(pos, size, resourceManager::getTexture("wall"), glm::vec2(0, 0), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f);
				obj.doCollisions = true;
				walls.push_back(obj);
			}
			if (tileData[y][x] == 2)
			{
				object obj(pos, size, resourceManager::getTexture("coin"), glm::vec2(0, 0), glm::vec3(1.0f,1.0f,1.0f), 0.0f);
				obj.doCollisions = true;
				coins.push_back(obj);
			}
			if (tileData[y][x] == 3)
			{
				object obj(pos, size, resourceManager::getTexture("flag"), glm::vec2(0, 0), glm::vec3(1.0f), 0.0f);
				obj.doCollisions = true;
				flags.push_back(obj);
			}
			if (tileData[y][x] == 4)
			{
				enemy obj(pos, size, glm::vec2(50, 0), resourceManager::getTexture("coin"));
				obj.initialPos = pos;
				obj.doCollisions = true;
				obj.type = 0;
				enemies.push_back(obj);
			}
			//Add Types here, e.g. enemy, different blocks, etc.
		}
	}
}

void level::draw(spriteRenderer &renderer)
{
	//std::cout << walls.size() << std::endl;
	for (int i = 0; i < walls.size(); i++)
	{
		object tile = walls[i];
		//std::cout << "Tile Rendered" << std::endl;
		if (!tile.destroyed)
		{
			tile.draw(renderer);
		}
	}
	for (int i = 0; i < coins.size(); i++)
	{
		object tile = coins[i];
		if (!tile.destroyed)
		{
			tile.draw(renderer);
		}
	}
	for (int i = 0; i < flags.size(); i++)
	{
		object tile = flags[i];
		if (!tile.destroyed)
		{
			tile.draw(renderer);
		}
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		enemy tile = enemies[i];
		if (!tile.destroyed)
		{
			tile.draw(renderer);
		}
	}
}
