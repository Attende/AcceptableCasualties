#pragma once

#include "glad/glad.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {

public:
	unsigned int shaderProgram;
	Shader();
	Shader(const char* vp, const char* fs);
	Shader &use();
	void setBool(const std::string &name, bool value);
	void setInt(const std::string &name, int value);
	void setFloat(const std::string &name, float value);
	void setVector2f(const std::string &name, glm::vec2 &value);
	void setVector2f(const std::string &name, float x, float y);
	void setVector3f(const std::string &name, glm::vec3 &value);
	void setVector3f(const std::string &name, float x, float y, float z);
	void setVector4f(const std::string &name, glm::vec4 &value);
	void setVector4f(const std::string &name, float x, float y, float z, float w);
	void setMatrix4(const std::string &name, glm::mat4 &value);
private:
	void checkCompilation(int object, std::string type);


};