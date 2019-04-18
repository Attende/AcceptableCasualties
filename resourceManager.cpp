#include "resourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
std::map<std::string, Shader>	resourceManager::Shaders;
std::map<std::string, Texture>	resourceManager::Textures;

Shader resourceManager::loadShader(const char *vShaderFile, const char *fShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
	return Shaders[name];
}

Shader resourceManager::getShader(std::string name)
{
	return Shaders[name];
}

Texture resourceManager::loadTexture(const char *textureFile, bool alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(textureFile, alpha);
	return Textures[name];
}

Texture resourceManager::getTexture(std::string name)
{
	return Textures[name];
}

Shader resourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile)
{
	std::string vCode, fCode;
	std::ifstream vFile, fFile;
	vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	//Reading files to obtain shader code
	try
	{
		vFile.open(vShaderFile);
		fFile.open(fShaderFile);
		std::stringstream vStream, fStream;
		vStream << vFile.rdbuf();
		fStream << fFile.rdbuf();
		vFile.close();
		fFile.close();
		vCode = vStream.str();
		fCode = fStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Error reading shader files" << std::endl;
	}
	const char* vsCode = vCode.c_str();
	const char* fsCode = fCode.c_str();
	Shader shader = Shader(vsCode, fsCode);
	return shader;
}



Texture resourceManager::loadTextureFromFile(const char *textureFile, bool alpha)
{
	Texture texture;
	//stbi_set_flip_vertically_on_load(true);
	if (alpha)
	{
		texture.InternalFormat = GL_RGBA;
		texture.ImageFormat = GL_RGBA;
	}
	int width, height, nrChannels;
	unsigned char* image = stbi_load(textureFile, &width, &height, &nrChannels, 0);
	texture.Generate(width, height, image);
	stbi_image_free(image);
	return texture;
}

std::vector<std::vector<unsigned int>> resourceManager::loadLevel(const char *levelFile)
{
	unsigned int tileInfo;
	std::string line;
	std::ifstream fstream(levelFile);
	std::vector<std::vector<unsigned int>> tileData;
	if (fstream)
	{
		while (std::getline(fstream, line))
		{
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			while (sstream >> tileInfo)
			{
				row.push_back(tileInfo);
			}
			tileData.push_back(row);
		}
		if(tileData.size() > 0) return tileData;
		else std::cout << "TileData Size is Zero" << std::endl;
	}
}


void resourceManager::Clear()
{
	for (auto iter : Shaders) { glDeleteProgram(iter.second.shaderProgram); }
	for (auto iter : Textures) { glDeleteProgram(iter.second.id); }
}