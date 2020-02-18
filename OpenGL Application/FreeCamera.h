#pragma once

#include "Camera.h"

class FreeCamera : public Camera
{
public:
	FreeCamera();
	//FreeCamera(
	//	const vec3 a_position = vec3(0.0f, 0.0f, 3.0f),
	//	const vec3 front = vec3(0.0f, 0.0f, -1.0f),
	//	const vec3 up = vec3(0.0f, 1.0f, 0.0f),
	//	const float fov = glm::radians(45.0f),
	//	const float aspect_ratio = 16.0 / 9.0f,
	//	const float near = 0.1f,
	//	const float far = 100.0f);

	void update(float a_delta_time);

	void set_speed(float a_speed);

private:
	float speed = 2.5f;
	float rotation_speed = 0.1f;
};

