#include "textRenderer.h"
#include <ft2build.h>
#pragma comment(lib, "freetyped.lib")
#include FT_FREETYPE_H

textRenderer::textRenderer(Shader _shader, unsigned int width, unsigned int height, std::string font, unsigned int fontSize)
{
	glm::mat4 temp = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f);
	_shader.use();
	_shader.setMatrix4("projection",temp);
	_shader.setInt("text", 0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader = _shader;
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "FreeType initialization failed" << std::endl;
	}
	FT_Face face;
	if (FT_New_Face(ft, font.c_str(), 0, &face))
	{
		std::cout << "Font load failed" << std::endl;
	}
	FT_Set_Pixel_Sizes(face, 0, fontSize);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (int i = 0; i < 128; i++)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			std::cout << "Glyph load failed" << std::endl;
		}
		Texture tex;
		tex.Filter_Min = GL_LINEAR;
		tex.Filter_Mag = GL_LINEAR;
		tex.Wrap_S = GL_CLAMP_TO_EDGE;
		tex.Wrap_T = GL_CLAMP_TO_EDGE;
		tex.ImageFormat = GL_RED;
		tex.InternalFormat = GL_RED;
		tex.Generate(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);
		character _character =
		{
			tex.id,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		characters.insert(std::pair<char, character>(i, _character));

	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glBindVertexArray(quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void textRenderer::renderText(std::string text, float x, float y, float scale, glm::vec3 colour)
{
	shader.use();
	shader.setVector3f("textColour", colour);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(quadVAO);
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		character ch = characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y + (characters['H'].Bearing.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 },
			{ xpos,     ypos,       0.0, 0.0 },

			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 }
		};
		
		glBindTexture(GL_TEXTURE_2D, ch.textID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);	
		
		x += (ch.hOffset >> 6) * scale; 
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}