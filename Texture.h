#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>


class Texture
{
public:
	GLuint id;
	GLuint width;
	GLuint height;
	GLuint InternalFormat;
	GLuint ImageFormat;

	GLuint Wrap_S;
	GLuint Wrap_T;
	GLuint Filter_Min;
	GLuint Filter_Mag;
	Texture();
	void Generate(GLuint width, GLuint height, unsigned char* data);
	void Bind();
	Texture &use();
private:
};


