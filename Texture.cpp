
#include "Texture.h"
Texture::Texture() : width(0), height(0), InternalFormat(GL_RGB), ImageFormat(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Mag(GL_LINEAR)
{
	glGenTextures(1, &id);
}

void Texture::Generate(GLuint width, GLuint height,  unsigned char* data) 
{

	glBindTexture(GL_TEXTURE_2D, id);
	//Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
	//Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Mag);

	glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, ImageFormat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
};
Texture &Texture::use()
{
	return *this;
}
void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

