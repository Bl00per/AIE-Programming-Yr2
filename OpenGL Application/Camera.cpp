#include "Camera.h"


void Camera::update(float a_delta_time)
{
	// 
}

void Camera::set_perspective(float fov, float aspect_ratio, float near, float far)
{
	projection_transform = glm::perspective(
		glm::radians(fov),
		aspect_ratio,
		near,
		far
	);
}

void Camera::set_perspective_ortho(float left, float right, float bottom, float top, float near, float far)
{
	projection_transform = glm::ortho(
		left,
		right,
		bottom,
		top,
		near,
		far
	);
}

void Camera::set_lookat(vec3 from, vec3 to, vec3 up)
{
	view_transform = glm::lookAt(
		from,
		to,
		up
	);
}

void Camera::set_position(vec3 position)
{
	// 
}


mat4 Camera::get_world_transform()
{
	return world_transform;
}

mat4 Camera::get_view()
{
	return view_transform;
}

mat4 Camera::get_projection()
{
	return projection_transform;
}

mat4 Camera::get_projection_view()
{
	return projection_view_transform;
}