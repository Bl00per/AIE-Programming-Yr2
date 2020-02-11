#pragma once
#include "glfw3.h"
#include "glm.hpp"
#include "ext.hpp"

using mat4 = glm::mat4;
using vec3 = glm::vec3;

class Camera
{
public:
	mat4 world_transform;
	mat4 view_transform;
	mat4 projection_transform;
	mat4 projection_view_transform;

	void update(float a_delta_time);

	// Setters
	void set_perspective(float fov, float aspect_ratio, float near, float far);
	void set_perspective_ortho(float left, float right, float bottom, float top, float near, float far);
	void set_lookat(vec3 from, vec3 to, vec3 up);
	void set_position(vec3 position);

	// Getters
	mat4 get_world_transform();
	mat4 get_view();
	mat4 get_projection();
	mat4 get_projection_view();

	//void update_projection_view_transform();

private:
	float delta_time;
	float last_frame;
};

