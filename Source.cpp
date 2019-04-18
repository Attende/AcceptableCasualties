#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include "game.h"
#include "resourceManager.h"




using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
void mouseCallback(GLFWwindow* window, int button, int action, int mode);
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
game Platformer(SCREEN_WIDTH, SCREEN_HEIGHT);


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Acceptable Casualties", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW Window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwMakeContextCurrent(window);


	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);
	glfwSetKeyCallback(window, keyCallback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed GLAD initialization" << endl;
	}
	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	Platformer.Init();
	while (!glfwWindowShouldClose(window)) {
		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		Platformer.processInput(deltaTime);

		Platformer.Update(deltaTime);
		glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT);
		Platformer.Render();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
	return 0;

}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			Platformer.Keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			Platformer.Keys[key] = false;
		}
	}
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glm::vec2 pos = glm::vec2((xPos / width) * SCREEN_WIDTH, (yPos / height) * SCREEN_HEIGHT);
	Platformer.mousePos = pos;
}
void mouseCallback(GLFWwindow* window, int button, int action, int mode)
{
	if (button >= 0 && button < 12)
	{
		if (action == GLFW_PRESS)
		{
			Platformer.MouseKeys[button] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			Platformer.MouseKeys[button] = false;
		}
	}
}