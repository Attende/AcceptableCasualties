#pragma once
#include "Shader.h"
#include "Texture.h"

class spriteRenderer
{
public:
	spriteRenderer(Shader &shader);
	~spriteRenderer();
	void drawSprite(Texture texture, glm::vec2 pos, glm::vec2 size = glm::vec2(10, 10), float rotate = 0.0f, glm::vec3 colour = glm::vec3(1.0f));
	glm::mat4 tempTest;
private:
	Shader shader;
	unsigned int quadVAO;
	void initRenderData();


};