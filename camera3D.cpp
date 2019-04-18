#include "camera3D.h"
camera::camera(vec3 _pos = vec3(0.0f, 0.0f, 0.0f), vec3 _up = vec3(0.0f, 1.0f, 0.0f), float _yaw = YAW, float _pitch = PITCH) 
	: camFront(vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED)
{
	pos = _pos;
	up = _up;
	yaw = _yaw;
	pitch = _pitch;
	update();
}
camera::camera(float pX, float pY, float pZ, float uX, float uY, float uZ, float _yaw, float _pitch)
	: camFront(vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED)
{
	pos = vec3(pX, pY, pZ);
	up = vec3(uX, uY, uZ);
	yaw = _yaw;
	pitch = _pitch;
	update();
}
mat4 camera::getViewMatrix()
{
	return lookAt(pos, pos + camFront, camUp);
}

void camera::keyboard(int direction, float dt)
{
	float vel = movementSpeed * dt;
	//Movement
	if (direction == 1) //Forwards
	{
		pos += vel * camUp;
	}
	if (direction == 2) //Backwards
	{
		pos -= vel * camUp;
	}
	if (direction == 3) //Left
	{
		pos -= vel * camRight;
	}
	if (direction == 4) //Right
	{
		pos += vel * camRight;
	}
	//Look Direction
	if (direction == 5) //Left
	{
		yaw += YAWSPEED * dt;
	}
	if (direction == 6) //Right
	{
		yaw -= YAWSPEED * dt;
	}
	if (direction == 7) //Up
	{
		pitch += PITCHSPEED * dt;
	}
	if (direction == 8) //Down
	{
		pitch -= PITCHSPEED * dt;
	}
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;
	pos.z = 0.0f;
	update();
}
void camera::mouseScroll(float yOffset)
{
	if (zoom >= 1.0f && zoom <= 45.0f) zoom -= yOffset;
	if (zoom < 1.0f) zoom = 1.0f;
	if (zoom > 45.0f) zoom = 45.0f;
}

void camera::update()
{
	vec3 tempFront;
	tempFront.x = cos(radians(yaw)) * cos(radians(pitch));
	tempFront.y = sin(radians(pitch));
	tempFront.z = sin(radians(yaw)) * cos(radians(pitch));
	camFront = normalize(tempFront);
	camRight = normalize(cross(camFront, up));
	camUp = normalize(cross(camRight, camFront));

}