#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 250.0f;
const float ZOOM = 45.0f;
const float YAWSPEED = 45.0f;
const float PITCHSPEED = 45.0f;

class camera
{
public:
	vec3 pos;
	vec3 camFront;
	vec3 camUp;
	vec3 camRight;
	vec3 up;
	//Angles
	float yaw;
	float pitch;
	//Camera Options
	float movementSpeed;

	float zoom;

	camera(vec3 _pos, vec3 _up, float _yaw, float _pitch);
	camera(float pX, float pY, float pZ, float uX, float uY, float uZ, float _yaw, float _pitch);
	mat4 getViewMatrix();

	void keyboard(int direction, float dt);
	void mouseScroll(float yOffset);

private:
	void update();
};