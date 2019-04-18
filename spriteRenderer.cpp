#include "spriteRenderer.h"

spriteRenderer::spriteRenderer(Shader &shader)
{
	this->shader = shader;
	initRenderData();
}
spriteRenderer::~spriteRenderer()
{
	glDeleteVertexArrays(1, &quadVAO);
}

void spriteRenderer::drawSprite(Texture texture, glm::vec2 pos, glm::vec2 size, float rotate, glm::vec3 colour)
{
	shader.use();
	glm::mat4 model = { {1.0f,0.0f,0.0f,0.0f},{0.0f,1.0f,0.0f,0.0f},{0.0f,0.0f,1.0f,0.0f},{0.0f,0.0f,0.0f,1.0f} };

	//Translation

	model = glm::translate(model, glm::vec3(pos, 0.0f));

	//Rotation

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	//Scaling
	model = glm::scale(model, glm::vec3(size, 1.0f));

	//Testing
	tempTest = model;

	this->shader.setMatrix4("model", model);
	this->shader.setVector3f("spriteColour", colour);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void spriteRenderer::initRenderData()
{
	unsigned int VBO;

	float vertices[] = {
		// Positions		// Texture Positions
		0.0f,	1.0f,		0.0f,	1.0f,
		1.0f,	0.0f,		1.0f,	0.0f,
		0.0f,	0.0f,		0.0f,	0.0f,

		0.0f,	1.0f,		0.0f,	1.0f,
		1.0f,	1.0f,		1.0f,	1.0f,
		1.0f,	0.0f,		1.0f,	0.0f
	};


	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT , GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}
/*			//Test Code for model matrix
for (int i = 0; i < 4; i++)
	{
		glm::vec4 row = model[i];
		for (int j = 0; j < 4; j++)
		{
			std::cout << "(" << i << "," << j << ") = " << row[j] << std::endl;
		}
	}

*/