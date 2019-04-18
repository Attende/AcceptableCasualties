#include "Shader.h"
using namespace std;
Shader::Shader() {}
Shader::Shader(const char* vs, const char* fs)
{

	//Vertex Shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vs, NULL);
	glCompileShader(vertexShader);
	checkCompilation(vertexShader, "VERTEX");

	//Fragment Shader

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fs, NULL);
	glCompileShader(fragmentShader);
	checkCompilation(fragmentShader, "FRAGMENT");

	
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	checkCompilation(shaderProgram, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader &Shader::use()
{
	glUseProgram(shaderProgram);
	return *this;
}

void Shader::setBool(const std::string &name, bool value)
{
	glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value)
{
	glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}
void Shader::setFloat(const std::string &name, float value)
{
	glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}
void Shader::setVector2f(const std::string &name, glm::vec2 &value)
{
	glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), value.x, value.y);
}
void Shader::setVector2f(const std::string &name, float x, float y)
{
	glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), x, y);
}
void Shader::setVector3f(const std::string &name, glm::vec3 &value)
{
	glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), value.x, value.y,value.z);
}
void Shader::setVector3f(const std::string &name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z);
}
void Shader::setVector4f(const std::string &name, glm::vec4 &value)
{
	glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), value.x, value.y, value.z,value.w);
}
void Shader::setVector4f(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z, w);
}
void Shader::setMatrix4(const std::string &name, glm::mat4 &value)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}


void Shader::checkCompilation(int object, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "Compilation error of type :" << type << "\n" << infoLog << "\n \n" << std::endl;
		}
	}
	else
	{
		glGetShaderiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "Link Error of type : " << type << "\n" << infoLog << "\n \n" << std::endl;

		}
	}
}