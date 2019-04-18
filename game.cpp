#include "game.h"
#include "resourceManager.h"
#include "spriteRenderer.h"
#include "textRenderer.h"
#include "camera2D.h"
#include "player.h"



using namespace glm;
Collision isColliding(object &obj1, object &obj2);
bool isCollidingWO(object &obj1, glm::vec2 offset1, object &obj2, glm::vec2 offset2);
direction collisionDirection4AABB(object &obj1, object &obj2);
direction collDirection3(object &obj1, object &obj2);
bool isClickingButton(glm::vec2 pos, glm::vec2 size, glm::vec2 mousePos);
//int keyBindMenu(glm::vec2 pos, glm::vec2 mousePos, std::string text, bool Keys[]);
//void resetPlayer(std::vector<level> &levels);
spriteRenderer *Renderer;
camera2d Camera = camera2d(glm::vec2(0.0f, 0.0f), 0.0f, 1.0f);
textRenderer *textRender;
textRenderer *fancyRenderer;
player *Player;

game::game(unsigned int width, unsigned int height)
{
	Width = width;
	Height = height;
};
game::~game()
{

};

void game::processCollisions(glm::vec2 initialPos)
{
	bool collisionOccured = false;
	for (object &wall : levels[Level].walls)
	{
		if (wall.doCollisions)
		{


			
			Collision collision = isColliding(*Player, wall);
			direction collision2 = collDirection3(*Player, wall);
			if (std::get<0>(collision))
			{
				direction compassDir = collision2;
				glm::vec2 vectorDir = std::get<2>(collision);
				if (compassDir == left || compassDir == right && resolveDirection == 0)
				{
					
					if (compassDir == right)
					{
						if (Player->vel.x > 0)
						{
							Player->vel.x = 0;
							Player->pos.x = initialPos.x;
						}
					}

					if (compassDir == left)
					{
						if (Player->vel.x < 0)
						{
							Player->vel.x = 0;
							Player->pos.x = initialPos.x;
						}
					}
				}
				else if (compassDir == up || compassDir == down && resolveDirection == 1)
				{
					if (compassDir == up)
					{
						if (Player->vel.y < 0)
						{
							Player->vel.y = 0;
							Player->pos.y = initialPos.y;
						}
					}
					if (compassDir == down)
					{
						collisionOccured = true;
						Player->collidedLastFrame = true;
						if (Player->vel.y > 0)
						{
							Player->vel.y = 0;
							Player->pos.y = initialPos.y;
						}
					}
				}

			}
			
		}
	}
	if (!collisionOccured)
	{
		Player->collidedLastFrame = false;
	}

	if (Player->canTakeDamage)
	{
		for (object &enemy : levels[Level].enemies)
		{
			if (enemy.doCollisions)
			{
				Collision collision = isColliding(*Player, enemy);
				direction collision2 = collDirection3(*Player, enemy);
				if (std::get<0>(collision))
				{
					if (collision2 == down)
					{
						Player->score += 200;
						enemy.destroyed = true;
						enemy.doCollisions = false;
						Player->vel.y = 0;
					}
					else
					{
						Player->hp -= 1;
						Player->canTakeDamage = false;
						std::cout << Player->hp << std::endl;
					}
				}
			}
		}
	}

	for (object &coin : levels[Level].coins)
	{
		if (coin.doCollisions)
		{
			Collision collision = isColliding(*Player, coin);
			if (std::get<0>(collision))
			{
				coin.destroyed = true;
				coin.doCollisions = false;
				Player->score += 100;
			}
		}
	}


	for (object &flag : levels[Level].flags)
	{
		if (flag.doCollisions)
		{
			//std::cout << "Flag Collision" << std::endl;
			Collision collision = isColliding(*Player, flag);
			if (std::get<0>(collision))
			{
				//std::cout << "Next Level" << std::endl;
				levels[Level].finished = true;
				Player->restart(glm::vec2(0.0f), glm::vec2(0.0f));
			}
		}
	}
}

void game::processEnemyCollisions(enemy &enemy, glm::vec2 initialPos)
{
	if (enemy.doCollisions)
	{
		bool collisionOccured = false;
		for (object &wall : levels[Level].walls)
		{
			if (wall.doCollisions)
			{
				Collision collision = isColliding(enemy, wall);
				direction collision2 = collDirection3(enemy, wall);
				glm::vec2 vectorDir = std::get<2>(collision);
				if (std::get<0>(collision))
				{
					if (collision2 == left || collision2 == right)
					{
						enemy.pos = initialPos;
						if (enemy.Horizontal == enemy::hor(0))
						{
							enemy.Horizontal = enemy::hor(1);
						}
						else if (enemy.Horizontal == enemy::hor(1))
						{
							enemy.Horizontal = enemy::hor(0);
						}
					}
					else if (collision2 == up || collision2 == down)
					{
						float depth = enemy.size.y / 2 - std::abs(vectorDir.y) + wall.size.y / 2;
						enemy.vel.y = 0;
						if (collision2 == down) {
							collisionOccured = true;
							enemy.collidedLastFrame = true;
							enemy.pos = glm::vec2(enemy.pos.x, enemy.pos.y - (depth));
						}
						else {
							enemy.pos = glm::vec2(enemy.pos.x, enemy.pos.y + (depth));
						}
					}

				}
			}
		}
		if (!collisionOccured)
		{
			enemy.collidedLastFrame = false;
		}
	}
}

void game::Init()
{
	textRender = new textRenderer(resourceManager::loadShader("../Graphics/Shaders/text.vs", "../Graphics/Shaders/text.fs", "text"),
		Width, Height, "C:/Windows/Fonts/arial.ttf", 48);

	fancyRenderer = new textRenderer(resourceManager::loadShader("../Graphics/Shaders/text.vs", "../Graphics/Shaders/text.fs", "text"),
		Width, Height, "C:/Windows/Fonts/castelar.ttf", 44);
	resourceManager::loadShader("../Graphics/Shaders/test.vs", "../Graphics/Shaders/test.fs", "sprite");


	//Projection Matrix
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);
	ProjectionMat = projection;


	//View Matrix
	Camera.width = Width;
	Camera.height = Height;
	mat4 view = Camera.getViewMatrix();
	std::cout << Camera.pos.x << "," << Camera.pos.y << std::endl;
	state = MENU;

	//TESTING STUFF





	//Sprite & Texture setup
	resourceManager::getShader("sprite").use();
	resourceManager::getShader("sprite").use().setInt("image", 0);
	resourceManager::getShader("sprite").setMatrix4("projection", projection);
	resourceManager::getShader("sprite").setMatrix4("view", view);
	resourceManager::loadTexture("../Graphics/Assets/Art/04mura.png", true, "wall");
	resourceManager::loadTexture("../Graphics/Assets/Art/rainbowBall.png", true, "ball");
	resourceManager::loadTexture("../Graphics/Assets/Art/8bitcity.jpg", false, "city");
	resourceManager::loadTexture("../Graphics/Assets/Art/coin.png", true, "coin");
	resourceManager::loadTexture("../Graphics/Assets/Art/flag.png", true, "flag");
	resourceManager::loadTexture("../Graphics/Assets/Art/walls/wall1.png", true, "wall1");
	resourceManager::loadTexture("../Graphics/Assets/Art/walls/more/14.jpg", false, "wall10");
	resourceManager::loadTexture("../Graphics/Assets/Art/menus/MAINMENU.png", true, "mainMenu");
	resourceManager::loadTexture("../Graphics/Assets/Art/menus/HELPMENUGAME.png", true, "helpMenu");
	resourceManager::loadTexture("../Graphics/Assets/Art/menus/STANDARDBACK.png", true, "backMenu");
	Renderer = new spriteRenderer(resourceManager::getShader("sprite").use());
	//Levels
	level one; 
	one.init(resourceManager::loadLevel("../levels/test/one.txt"),resourceManager::getTexture("wall"), Width, Height);
	levels.push_back(one);
	level two;
	two.init(resourceManager::loadLevel("../levels/test/two.txt"), resourceManager::getTexture("wall"), Width, Height);
	levels.push_back(two);
	level three;
	three.init(resourceManager::loadLevel("../levels/test/three.txt"), resourceManager::getTexture("wall"), Width, Height);
	levels.push_back(three);
	Level = 1;
	//Player Setup
	Player = new player(glm::vec2(50,50), glm::vec2(100, 75), glm::vec2(10, -10), resourceManager::getTexture("ball"));
};
void game::processInput(float dt)
{
	if (state == ACTIVE)
	{
		resetTimer += dt;

		if (Player->collidedLastFrame)
		{
			if (Keys[jumpKey] == GLFW_PRESS && Keys[highJumpKey] == GLFW_PRESS) Player->jump(-1200);
			else if (Keys[jumpKey] == GLFW_PRESS) Player->jump(-800);
		}
		if (Keys[resetKey] == GLFW_PRESS && resetTimer > 0.2)
		{
			resetTimer = 0;
			Player->dead = true;
		}
		if (Keys[GLFW_KEY_HOME] == GLFW_PRESS) state = MENU;
		if (Keys[rightKey] == GLFW_PRESS) Player->deltav(dt, 1, 2);
		if (Keys[leftKey] == GLFW_PRESS) Player->deltav(dt, 2, 2);

		if (Keys[leftKey] == GLFW_RELEASE && Keys[rightKey] == GLFW_RELEASE && Player->collidedLastFrame)
		{
			if (Player->vel.x != 0)
			{
				Player->vel.x = Player->vel.x * 0.6;
			}
		}
		//if (Keys[GLFW_KEY_S] == GLFW_PRESS) Player->deltav(dt, 3, 2);
		//if (Keys[GLFW_KEY_W] == GLFW_PRESS) Player->deltav(dt, 4, 3);

		if (!Player->collidedLastFrame) Player->deltav(dt, 3, 2);
		//if (Keys[GLFW_KEY_KP_8] == GLFW_PRESS) Camera.keyboard(1, dt); //Redundant
		//if (Keys[GLFW_KEY_KP_2] == GLFW_PRESS) Camera.keyboard(2, dt);
		//if (Keys[GLFW_KEY_KP_4] == GLFW_PRESS) Camera.keyboard(3, dt);
		//if (Keys[GLFW_KEY_KP_6] == GLFW_PRESS) Camera.keyboard(4, dt); //End of Redundant
		if (Keys[GLFW_KEY_LEFT] == GLFW_PRESS) Camera.keyboard(5, dt);	//Rotate
		if (Keys[GLFW_KEY_RIGHT] == GLFW_PRESS) Camera.keyboard(6, dt); //Rotate
		if (Keys[GLFW_KEY_UP] == GLFW_PRESS) Camera.keyboard(7, dt);	//Zoom In
		if (Keys[GLFW_KEY_DOWN] == GLFW_PRESS) Camera.keyboard(8, dt);	//Zoom Out
	}
	if (state == MENU)
	{

	}
};
void game::Update(float dt)
{
	if (state == ACTIVE)
	{
		animationTimer += dt;
		if (animationTimer >= 0.1)
		{
			//Swap Textures
		}
		if (levels[Level].finished)
		{
			if (Level < levels.size()) {
				Level += 1;
			}
		}
		if (firstUpdate)	//On first update, dt is rediculously high hence this is called so that the player doesn't 
							//fall through the map.
		{
			Player->pos = glm::vec2(0, 0);
			Camera.zoom = 0.5f;
			firstUpdate = false;
			dt = 0;
		}

		glm::vec2 playerPos = Player->pos;
		Player->xMove(dt, 0, 4000);
		resolveDirection = 0;
		processCollisions(playerPos);
		Player->yMove(dt, 0, 2000);
		resolveDirection = 1;
		processCollisions(playerPos);
		//std::cout << Width << "," << Height << std::endl;
		if (Player->pos.x < Width / (2 * Camera.zoom) && Player->pos.y < Height / (2 * Camera.zoom))
		{
			Camera.setPos(glm::vec2(-(Width*(1 - Camera.zoom)),-(Height*(1 - Camera.zoom))));
		}
		else if (Player->pos.x < Width / (2 * Camera.zoom))
		{
			Camera.setPos(glm::vec2(-(Width / 2), Height / 2 - Player->pos.y));
		}
		else if (Player->pos.y < Height / (2 * Camera.zoom))
		{
			Camera.setPos(glm::vec2(Width / 2 - (Player->pos.x), -(Height / 2)));
		}
		else
		{
			Camera.setPos(glm::vec2(Width / 2 - (Player->pos.x),Height / 2 - Player->pos.y));
		}

		for (enemy &en : levels[Level].enemies)
		{
			glm::vec2 initPos = en.pos;
			en.xMove2(dt);
			resolveDirection = 0;
			processEnemyCollisions(en,initPos);
			en.vel = glm::vec2(en.vel.x, en.vel.y + 1600 * dt);
			en.yMove(dt, 0, 2000);
			resolveDirection = 1;
			processEnemyCollisions(en,initPos);
			if (en.collidedLastFrame)
			{
				if (Player->pos.y < en.pos.y)
				{
					std::cout << "Can Jump" << std::endl;
					en.jump(-800);
					en.yMove(dt,0,2000);
				}
			}
		}
		
		if (!Player->canTakeDamage)
		{
			damageTimer += dt;
			if (damageTimer > 1)
			{
				Player->canTakeDamage = true;
				damageTimer = 0;
			}
		}
		if (Player->hp <= 0)
		{
			Player->dead = true;
		}
		if (Player->dead)
		{
			if (Player->lives <= 0)
			{
				state = GAMEOVER;
				Player->hp = 2;
			}
			else
			{
				Player->lives -= 1;
				Player->hp = 2;
				Player->restart(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f));
				for (int i = 0; i < levels.size(); i++)
				{
					for (int j = 0; j < levels[i].enemies.size(); j++)
					{
						levels[i].enemies[j].pos = levels[i].enemies[j].initialPos;
					}
				}
			}
		}
	}
	if (state == MENU)
	{
		Camera.setPos(glm::vec2(0, 0));
	}
	if (state == GAMEOVER)
	{
		Camera.setPos(glm::vec2(0, 0));
	}
	if (restart == true)
	{
		Player->lives = 3;
		Player->hp = 2;
		Player->dead = false;
		for (int i = 0; i < levels.size(); i++)
		{
			levels[i].finished = false;
			for (int j = 0; j < levels[i].coins.size(); j++)
			{
				levels[i].coins[j].doCollisions = true;
				levels[i].coins[j].destroyed = false;
			}
			for (int j = 0; j < levels[i].enemies.size(); j++)
			{
				//std::cout << "Enemy Respawned" << std::endl;
				levels[i].enemies[j].destroyed = false;
				levels[i].enemies[j].doCollisions = true;
			}
			//std::cout << levels[i].coins.size() << std::endl;
		}
		Player->score = 0;
		Player->restart(glm::vec2(0.0f),glm::vec2(0.0f));
		firstUpdate = true;
		restart = false;
	}
};
void game::Render()
{
	if (state == ACTIVE)
	{
		Renderer->drawSprite(resourceManager::getTexture("city"), glm::vec2(-800, -600), glm::vec2(5600, 3200), 0, glm::vec3(1.0f, 1.0f, 1.0f));
		levels[Level].draw(*Renderer);
		if (Player->canTakeDamage)
		{
			Player->colour = glm::vec3(1.0f);
			Player->draw(*Renderer);
		}
		else
		{
			Player->colour = glm::vec3(1.0f,0.0f,0.0f);
			Player->draw(*Renderer);
		}

		mat4 view = Camera.getViewMatrix();
		resourceManager::getShader("sprite").setMatrix4("view", view);

		std::string scoreLine = "Score: " + std::to_string(Player->score);
		textRender->renderText(scoreLine, 0.0f, 0.0f, 0.5f, vec3(1.0f));

		std::string livesLine = "Lives: " + std::to_string(Player->lives);
		textRender->renderText(livesLine, 0.0f, 30.0f, 0.5f, vec3(1.0f));
	}
	if (state == MENU)
	{

		Renderer->drawSprite(resourceManager::getTexture("mainMenu"), glm::vec2(0, 0), glm::vec2(800, 600), 0, glm::vec3(1.0f, 1.0f, 1.0f));
		Camera.zoom = 1.0f;
		mat4 view = Camera.getViewMatrix();
		resourceManager::getShader("sprite").setMatrix4("view", view);

		glm::vec2 playPos = glm::vec2(450.0f, 273.0f);
		bool buttonPlay = isClickingButton(playPos, glm::vec2(300, 44), mousePos);
		if (buttonPlay)
		{
			fancyRenderer->renderText("Play Game", playPos.x, playPos.y, 1.0f, vec3(1.0f, 0.0f, 0.0f));
			if (MouseKeys[GLFW_MOUSE_BUTTON_1] == GLFW_PRESS)
			{
				std::cout << "occuring" << std::endl;
				state = ACTIVE;
			}
		}
		else
		{
			fancyRenderer->renderText("Play Game", playPos.x, playPos.y, 1.0f, vec3(0.0f));
		}


		glm::vec2 helpPos = glm::vec2(530.0f, 320.0f);
		bool buttonHelp = isClickingButton(helpPos, glm::vec2(135, 44), mousePos);
		if (buttonHelp)
		{
			fancyRenderer->renderText("Help", helpPos.x, helpPos.y, 1.0f, vec3(1.0f, 0.0f, 0.0f));
			if (MouseKeys[GLFW_MOUSE_BUTTON_1] == GLFW_PRESS)
			{
				std::cout << "occuring" << std::endl;
				state = HELP;
			}
		}
		else
		{
			fancyRenderer->renderText("Help", helpPos.x, helpPos.y, 1.0f, vec3(0.0f));
		}

		glm::vec2 settingsPos = glm::vec2(485.0f, 365.0f);
		bool buttonSettings = isClickingButton(settingsPos, glm::vec2(230, 44), mousePos);
		if (buttonSettings)
		{
			fancyRenderer->renderText("Settings", settingsPos.x, settingsPos.y, 1.0f, vec3(1.0f, 0.0f, 0.0f));
			if (MouseKeys[GLFW_MOUSE_BUTTON_1] == GLFW_PRESS)
			{
				std::cout << "occuring" << std::endl;
				state = SETTINGS;
			}
		}
		else
		{
			fancyRenderer->renderText("Settings", settingsPos.x, settingsPos.y, 1.0f, vec3(0.0f));
		}
	}
	if (state == HELP)
	{
		Renderer->drawSprite(resourceManager::getTexture("helpMenu"), glm::vec2(0, 0), glm::vec2(800, 600), 0, glm::vec3(1.0f, 1.0f, 1.0f));
		glm::vec2 playPos = glm::vec2(20.0f, 540.0f);
		bool buttonPlay = isClickingButton(playPos, glm::vec2(410, 48), mousePos);
		if (buttonPlay)
		{
			fancyRenderer->renderText("Return to Menu", playPos.x, playPos.y, 0.75f, vec3(1.0f, 0.0f, 0.0f));
			if (MouseKeys[GLFW_MOUSE_BUTTON_1] == GLFW_PRESS)
			{
				std::cout << "occuring" << std::endl;
				state = MENU;
			}
		}
		else
		{
			fancyRenderer->renderText("Return to Menu", playPos.x, playPos.y, 0.75f, vec3(0.0f));
		}

	}
	if (state == SETTINGS)
	{
		Renderer->drawSprite(resourceManager::getTexture("mainMenu"), glm::vec2(0, 0), glm::vec2(800, 600), 0, glm::vec3(1.0f, 1.0f, 1.0f));
		fancyRenderer->renderText("Settings", 100.0f, 100.0f, 0.9f, vec3(1.0f, 1.0f, 1.0f));
		//return button
		glm::vec2 playPos = glm::vec2(20.0f, 540.0f);
		bool buttonPlay = isClickingButton(playPos, glm::vec2(410, 48), mousePos);
		if (buttonPlay)
		{
			fancyRenderer->renderText("Return to Menu", playPos.x, playPos.y, 0.75f, vec3(1.0f, 0.0f, 0.0f));
			if (MouseKeys[GLFW_MOUSE_BUTTON_1] == GLFW_PRESS)
			{
				std::cout << "occuring" << std::endl;
				state = MENU;
			}
		}
		else
		{
			fancyRenderer->renderText("Return to Menu", playPos.x, playPos.y, 0.75f, vec3(0.0f));
		}

		//Keybind menu button
		glm::vec2 keybindingPos = glm::vec2(480.0f, 273.0f);
		bool buttonKeybinds = isClickingButton(keybindingPos, glm::vec2(300, 44), mousePos);
		if (buttonKeybinds)
		{
			fancyRenderer->renderText("Keybinds", keybindingPos.x, keybindingPos.y, 1.0f, vec3(1.0f, 0.0f, 0.0f));
			if (MouseKeys[GLFW_MOUSE_BUTTON_1] == GLFW_PRESS)
			{
				std::cout << "occuring" << std::endl;
				state = KEYBINDS;
			}
		}
		else
		{
			fancyRenderer->renderText("Keybinds", keybindingPos.x, keybindingPos.y, 1.0f, vec3(0.0f));
		}


		//Misc menu button
		glm::vec2 miscPos = glm::vec2(530.0f, 320.0f);
		bool buttonMisc = isClickingButton(miscPos, glm::vec2(135, 44), mousePos);
		if (buttonMisc)
		{
			fancyRenderer->renderText("Misc", miscPos.x, miscPos.y, 1.0f, vec3(1.0f, 0.0f, 0.0f));
			if (MouseKeys[GLFW_MOUSE_BUTTON_1] == GLFW_PRESS)
			{
				std::cout << "occuring" << std::endl;
				state = MISC;
			}
		}
		else
		{
			fancyRenderer->renderText("Misc", miscPos.x, miscPos.y, 1.0f, vec3(0.0f));
		}
	}
	if (state == KEYBINDS)
	{
		Renderer->drawSprite(resourceManager::getTexture("mainMenu"), glm::vec2(0, 0), glm::vec2(800, 600), 0, glm::vec3(1.0f, 1.0f, 1.0f));
		fancyRenderer->renderText("Keybinds", 90.0f, 100.0f, 0.9f, vec3(1.0f, 1.0f, 1.0f));
		glm::vec2 playPos = glm::vec2(20.0f, 540.0f);
		bool buttonPlay = isClickingButton(playPos, glm::vec2(410, 48), mousePos);
		if (buttonPlay)
		{
			fancyRenderer->renderText("Return to Menu", playPos.x, playPos.y, 0.75f, vec3(1.0f, 0.0f, 0.0f));
			if (MouseKeys[GLFW_MOUSE_BUTTON_1] == GLFW_PRESS)
			{
				std::cout << "occuring" << std::endl;
				state = MENU;
			}
		}
		else
		{
			fancyRenderer->renderText("Return to Menu", playPos.x, playPos.y, 0.75f, vec3(0.0f));
		}

		//Right Key Change
		glm::vec2 fPos = glm::vec2(90.0f, 200.0f);
		fancyRenderer->renderText("Right", fPos.x, fPos.y, 0.6f, vec3(1.0f, 1.0f, 1.0f));
		bool fKey = isClickingButton(glm::vec2(fPos.x + 200.0f, fPos.y), glm::vec2(100, 30), mousePos);
		if (fKey)
		{
			for (int i = 0; i < 1024; i++)
			{
				if (Keys[i] == GLFW_PRESS)
				{
					rightKey = i;
				}

			}
			fancyRenderer->renderText(glfwGetKeyName(rightKey, 0), fPos.x + 200.0f, fPos.y, 0.75f, vec3(1.0f, 0.0f, 0.0f));
		}
		else
		{
			fancyRenderer->renderText(glfwGetKeyName(rightKey, 0), fPos.x + 200.0f, fPos.y, 0.75f, vec3(1.0f, 1.0f, 1.0f));
		}
		//Left Key Change
		glm::vec2 bPos = glm::vec2(90.0f, 245.0f);
		fancyRenderer->renderText("Left", bPos.x, bPos.y, 0.6f, vec3(1.0f, 1.0f, 1.0f));
		bool bKey = isClickingButton(glm::vec2(bPos.x + 200.0f, bPos.y), glm::vec2(100, 30), mousePos);
		int prevLeftKey = leftKey;
		if (bKey)
		{
			for (int i = 0; i < 1024; i++)
			{
				if (Keys[i] == GLFW_PRESS)
				{
					leftKey = i;
				}
			}
			fancyRenderer->renderText(glfwGetKeyName(leftKey, 0), bPos.x + 200.0f, bPos.y, 0.75f, vec3(1.0f,0.0f,0.0f));
		}
		else
		{
			fancyRenderer->renderText(glfwGetKeyName(leftKey, 0), bPos.x + 200.0f, bPos.y, 0.75f, vec3(1.0f));
		}
		//Jump Key Change
		glm::vec2 jumpPos = glm::vec2(90.0f, 285.0f);
		fancyRenderer->renderText("Jump", jumpPos.x, jumpPos.y, 0.6f, vec3(1.0f, 1.0f, 1.0f));
		bool keyH = isClickingButton(glm::vec2(jumpPos.x + 200.0f, jumpPos.y), glm::vec2(100, 30), mousePos);
		if (keyH)
		{
			for (int i = 0; i < 1024; i++)
			{
				if (Keys[i] == GLFW_PRESS)
				{
					jumpKey = i;
				}
			}
			if (jumpKey == 32)
			{
				fancyRenderer->renderText("Space", jumpPos.x + 200.0f, jumpPos.y, 0.75f, vec3(1.0f, 0.0f, 0.0f));
			}
			else
			{
				fancyRenderer->renderText(glfwGetKeyName(jumpKey, 0), jumpPos.x + 200.0f, jumpPos.y, 0.75f, vec3(1.0f,0.0f,0.0f));
			}
		}
		else
		{
			if (jumpKey == 32)
			{
				fancyRenderer->renderText("Space", jumpPos.x + 200.0f, jumpPos.y, 0.75f, vec3(1.0f));
			}
			else
			{
				fancyRenderer->renderText(glfwGetKeyName(jumpKey, 0), jumpPos.x + 200.0f, jumpPos.y, 0.75f, vec3(1.0f));
			}
		}
		//High Jump Key
		glm::vec2 highPos = glm::vec2(90.0f, 330.0f);
		fancyRenderer->renderText("High Jump", highPos.x, highPos.y, 0.6f, vec3(1.0f, 1.0f, 1.0f));
		bool keyHigh = isClickingButton(glm::vec2(highPos.x + 200.0f, highPos.y), glm::vec2(130, 30), mousePos);
		if (keyHigh)
		{
			for (int i = 0; i < 1024; i++)
			{
				if (Keys[i] == GLFW_PRESS)
				{
					highJumpKey = i;
				}
			}
			if (highJumpKey == 32)
			{
				fancyRenderer->renderText("Space", highPos.x + 200.0f, highPos.y, 0.75f, vec3(1.0f, 0.0f, 0.0f));
			}
			else if (highJumpKey == 340)
			{
				fancyRenderer->renderText("L-Shift", highPos.x + 200.0f, highPos.y, 0.75f, vec3(1.0f, 0.0f, 0.0f));
			}
			else if (highJumpKey == 341)
			{
				fancyRenderer->renderText("L-Ctrl", highPos.x + 200.0f, highPos.y, 0.75f, vec3(1.0f, 0.0f, 0.0f));
			}
			else
			{
				fancyRenderer->renderText(glfwGetKeyName(highJumpKey, 0), highPos.x + 200.0f, highPos.y, 0.75f, vec3(1.0f, 0.0f, 0.0f));
			}
		}
		else
		{
			if (highJumpKey == 32)
			{
				fancyRenderer->renderText("Space", highPos.x + 200.0f, highPos.y, 0.75f, vec3(1.0f));
			}
			else if (highJumpKey == 340)
			{
				fancyRenderer->renderText("L-Shift", highPos.x + 200.0f, highPos.y, 0.75f, vec3(1.0f));
			}
			else if (highJumpKey == 341)
			{
				fancyRenderer->renderText("L-Ctrl", highPos.x + 200.0f, highPos.y, 0.75f, vec3(1.0f));
			}
			else
			{
				fancyRenderer->renderText(glfwGetKeyName(highJumpKey, 0), highPos.x + 200.0f, highPos.y, 0.75f, vec3(1.0f));
			}
		}

		//Reset Key
		glm::vec2 rPos = glm::vec2(90.0f, 370.0f);
		fancyRenderer->renderText("Reset", rPos.x, rPos.y, 0.6f, vec3(1.0f, 1.0f, 1.0f));
		bool rKey = isClickingButton(glm::vec2(rPos.x + 200.0f, rPos.y), glm::vec2(100, 30), mousePos);
		if (rKey)
		{
			for (int i = 0; i < 1024; i++)
			{
				if (Keys[i] == GLFW_PRESS)
				{
					resetKey = i;
				}
			}
			fancyRenderer->renderText(glfwGetKeyName(resetKey, 0), rPos.x + 200.0f, rPos.y, 0.75f, vec3(1.0f,0.0f,0.0f));
		}
		else
		{
			fancyRenderer->renderText(glfwGetKeyName(resetKey, 0), rPos.x + 200.0f, rPos.y, 0.75f, vec3(1.0f));
		}

		/*
		This shouldn't be in render loop, however it was easier to program without having to use globals, or passing
		variables between update and render. 
		Written out each time as functions failed to work correctly.
		
		*/
	}
	if (state == MISC)
	{
		Renderer->drawSprite(resourceManager::getTexture("mainMenu"), glm::vec2(0, 0), glm::vec2(800, 600), 0, glm::vec3(1.0f, 1.0f, 1.0f));
		fancyRenderer->renderText("Miscellaneous", 50.0f, 100.0f, 0.9f, vec3(1.0f, 1.0f, 1.0f));
		glm::vec2 playPos = glm::vec2(20.0f, 540.0f);
		bool buttonPlay = isClickingButton(playPos, glm::vec2(410, 48), mousePos);
		if (buttonPlay)
		{
			fancyRenderer->renderText("Return to Menu", playPos.x, playPos.y, 0.75f, vec3(1.0f, 0.0f, 0.0f));
			if (MouseKeys[GLFW_MOUSE_BUTTON_1] == GLFW_PRESS)
			{
				std::cout << "occuring" << std::endl;
				state = MENU;
			}
		}
		else
		{
			fancyRenderer->renderText("Return to Menu", playPos.x, playPos.y, 0.75f, vec3(0.0f));
		}
	}
	if (state == GAMEOVER)
	{
		Renderer->drawSprite(resourceManager::getTexture("backMenu"), glm::vec2(0, 0), glm::vec2(800, 600), 0, glm::vec3(1.0f, 1.0f, 1.0f));
		Camera.zoom = 1.0f;
		mat4 view = Camera.getViewMatrix();
		resourceManager::getShader("sprite").setMatrix4("view", view);

		glm::vec2 gameOverPos = glm::vec2(250, 190);
		fancyRenderer->renderText("Game Over", gameOverPos.x, gameOverPos.y, 1.0f, glm::vec3(1.0f));
		restart = true;
		Level = 1;
		//Restart
		glm::vec2 restartPos = glm::vec2(50.0f, 415.0f);
		bool buttonRestart = isClickingButton(restartPos, glm::vec2(250, 48), mousePos);
		if (buttonRestart)
		{
			fancyRenderer->renderText("Restart Game", restartPos.x, restartPos.y, 0.7f, vec3(1.0f, 0.0f, 0.0f));
			if (MouseKeys[GLFW_MOUSE_BUTTON_1] == GLFW_PRESS)
			{
				state = ACTIVE;
			}
		}
		else
		{
			fancyRenderer->renderText("Restart Game", restartPos.x, restartPos.y, 0.7f, vec3(1.0f));
		}

		//Return to Menu
		glm::vec2 menuPos = glm::vec2(440.0f, 415.0f);
		bool buttonMenu = isClickingButton(menuPos, glm::vec2(330, 48), mousePos);
		if (buttonMenu)
		{
			fancyRenderer->renderText("Return to Menu", menuPos.x, menuPos.y, 0.7f, vec3(1.0f, 0.0f, 0.0f));
			if (MouseKeys[GLFW_MOUSE_BUTTON_1] == GLFW_PRESS)
			{
				state = MENU;
			}
		}
		else
		{
			fancyRenderer->renderText("Return to Menu", menuPos.x, menuPos.y, 0.7f, vec3(1.0f));
		}
		
	}
};

int keyBindMenu(glm::vec2 pos, glm::vec2 mousePos, std::string text, bool Keys[])
{
	fancyRenderer->renderText(text, pos.x, pos.y, 0.6f, vec3(1.0f, 1.0f, 1.0f));
	bool keyH = isClickingButton(glm::vec2(pos.x + 200.0f, pos.y), glm::vec2(50, 30), mousePos);
	if (keyH)
	{
		for (int i = 0; i < 1024; i++)
		{
			if (Keys[i] == GLFW_PRESS)
			{
				return i;
			}
		}
	}
}

direction collisionDirection(glm::vec2 target)
{
	glm::vec2 directions[] = {
		glm::vec2(0.0f,1.0f), //Up
		glm::vec2(0.0f,-1.0f), //Down
		glm::vec2(1.0f, 0.0f), //Left
		glm::vec2(-1.0f, 0.0f) //Right
	};
	float max = 0.0f;
	int match = -1;
	for (int i = 0; i < 4; i++)
	{
		float dot = glm::dot(glm::normalize(target), directions[i]);
		if (dot > max)
		{
			max = dot;
			match = i;
		}
	}
	return (direction)match;

}
direction collisionDirection4AABB(object &obj1, object &obj2)
{
	bool Up		 = isCollidingWO(obj1, vec2(0.0f, 1.0f), obj2, vec2(0.0f));
	bool Down	 = isCollidingWO(obj1, vec2(0.0f, -1.0f), obj2, vec2(0.0f));
	bool Left	 = isCollidingWO(obj1, vec2(1.0f, 0.0f), obj2, vec2(0.0f));
	bool Right	 = isCollidingWO(obj1, vec2(-1.0f, 0.0f), obj2, vec2(0.0f));
	if (Down) return down;
	if (Up) return up;
	if (Left) return left;
	if (Right) return right;
}

direction collDirection3(object &obj1, object &obj2)
{
	float pBottom = obj1.pos.y + obj1.size.y;
	float tBottom = obj2.pos.y + obj2.size.y;
	float pRight = obj1.pos.x + obj1.size.x;
	float tRight = obj2.pos.x + obj2.size.x;

	float bColl = tBottom - obj1.pos.y;
	float tColl = pBottom - obj2.pos.y;
	float lColl = pRight - obj2.pos.x;
	float rColl = tRight - obj1.pos.x;

	if (tColl < bColl && tColl < lColl && tColl < rColl) return down;
	if (bColl < tColl && bColl < lColl && bColl < rColl) return up;
	if (lColl < tColl && lColl < bColl && lColl < rColl) return right;
	if (rColl < tColl && rColl < bColl && rColl < lColl) return left;
}
Collision isColliding(object &obj1, object &obj2)
{	
	glm::vec2 centre = obj1.pos + glm::vec2(obj1.size.x / 2, obj1.size.y / 2);
	glm::vec2 centre2 = obj2.pos + glm::vec2(obj2.size.x / 2, obj2.size.y / 2);
	glm::vec2 difference = centre - centre2;
	bool xAxis = obj1.pos.x + obj1.size.x >= obj2.pos.x && obj2.pos.x + obj2.size.x >= obj1.pos.x;
	bool yAxis = obj1.pos.y + obj1.size.y >= obj2.pos.y && obj2.pos.y + obj2.size.y >= obj1.pos.y;

	return std::make_tuple(xAxis && yAxis, collisionDirection(difference),difference);

}
bool collisionCheck(object &obj1, object &obj2)
{
	bool xAxis = obj1.pos.x + obj1.size.x >= obj2.pos.x && obj2.pos.x + obj2.size.x >= obj1.pos.x;
	bool yAxis = obj1.pos.y + obj1.size.y >= obj2.pos.y && obj2.pos.y + obj2.size.y >= obj1.pos.y;
	return xAxis && yAxis;
}


bool isCollidingWO(object &obj1,glm::vec2 offset1, object &obj2, glm::vec2 offset2)
{
	bool xAxis = obj1.pos.x + obj1.size.x + offset1.x >= obj2.pos.x + offset2.x && obj2.pos.x + obj2.size.x + offset2.x >= obj1.pos.x + offset1.x;
	bool yAxis = obj1.pos.y + obj1.size.y + offset1.y >= obj2.pos.y + offset2.y && obj2.pos.y + obj2.size.y + offset2.y >= obj1.pos.y + offset1.y;
	return xAxis && yAxis;
}

bool isClickingButton(glm::vec2 pos, glm::vec2 size, glm::vec2 mousePos)
{
	float top = pos.y;
	float bottom = pos.y + size.y;
	float left = pos.x;
	float right = pos.x + size.x;
	if (mousePos.x > left && mousePos.x < right && mousePos.y > top && mousePos.y < bottom)
	{
		return true;
	}
	return false;
}
