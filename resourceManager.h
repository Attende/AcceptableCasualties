#pragma once
#include <map>
#include <string>
#include "Shader.h"
#include "Texture.h"
#include <iostream>
#include <fstream>
#include "level.h"


class resourceManager
{
public:

	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture> Textures;
	static Shader loadShader(const char *vShaderFile, const char *fShaderFile, std::string name);
	static Shader getShader(std::string name);
	static Texture loadTexture(const char *textureFile, bool alpha, std::string name);
	static Texture getTexture(std::string name);
	static std::vector<std::vector<unsigned int>> loadLevel(const char *levelFile);
	static void Clear();
private:
	static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile);
	static Texture loadTextureFromFile(const char *textureFile, bool alpha);
	
};