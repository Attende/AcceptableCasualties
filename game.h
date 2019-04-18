#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "level.h"
#include <tuple>


enum direction {
	up,
	down,
	left,
	right
};

typedef std::tuple<bool, direction, glm::vec2> Collision;

class game
{
public:
	float damageTimer = 0;
	float animationTimer = 0;
	float resetTimer = 0;
	glm::vec2 mousePos;
	int resolveDirection;
	bool firstUpdate = true;
	bool Keys[1024];
	bool MouseKeys[12];
	bool restart = false;
	enum gameState { MENU, HELP, SETTINGS, KEYBINDS, MISC, ACTIVE, GAMEOVER, GAMEWON };
	gameState state;
	std::vector<level> levels;
	unsigned int Level;
	int Width, Height;
	glm::mat4 ProjectionMat;
	game(unsigned int width, unsigned int height);
	~game();
	void processEnemyCollisions(enemy &enemy, glm::vec2 initialPos);
	void processCollisions(glm::vec2 initialPos);
	void Init();
	void processInput(float dt);
	void Update(float dt);
	void Render();
	int leftKey = GLFW_KEY_A;
	int rightKey = GLFW_KEY_D;
	int jumpKey = GLFW_KEY_SPACE;
	int highJumpKey = GLFW_KEY_LEFT_SHIFT;
	int resetKey = GLFW_KEY_R;
};