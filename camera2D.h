#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

const glm::vec2 SPEED = glm::vec2(100.0f);
const float ZOOM = 1.0f;
const float ZOOMSPEED = 2.0f;
const float ROT = 0.0f;
const float ROTSPEED = 1.0f;
const float ZOOMMAX = 5.0f;
const float ZOOMMIN = 0.5f;

class camera2d
{
public:
	//Camera information
	vec2 pos;
	glm::vec2 moveSpeed;
	float rot;
	float zoom;
	//Translation Information
	float xOffset;
	float yOffset;
	//Window information
	int width;
	int height;
	
	camera2d(vec2 pos, float rot, float zoom);
	void keyboard(int direction, float dt);
	void setPos(glm::vec2 pos);
	mat4 getViewMatrix();
private:
	void update();

};

