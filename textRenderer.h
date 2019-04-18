#pragma once
#include "Shader.h"
#include "Texture.h"
#include <map>
#include <string>
struct character {
	unsigned int textID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int hOffset; //Offset to next letter (REMEMBER)
};


class textRenderer
{
public:
	std::map<char, character> characters;
	Shader shader;
	unsigned int quadVAO;
	unsigned int VBO;
	textRenderer(Shader _shader, unsigned int width, unsigned int height, std::string font, unsigned int fontSize);
	void renderText(std::string text, float x, float y, float scale, glm::vec3 colour);
private:

};


