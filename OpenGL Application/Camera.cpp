#include "Camera.h"

Camera::Camera(
	float fov,
	float aspect_ratio,
	float near,
	float far)
{
	set_perspective(fov, aspect_ratio, near, far);
	set_lookat(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 10.0f, 0.0f));
}

void Camera::update(float a_delta_time)
{
}

void Camera::set_perspective(float fov, float aspect_ratio, float near, float far)
{
	projection_transform = glm::perspective(
		glm::radians(fov),
		aspect_ratio,
		near,
		far
	);

	update_projection_view_transform();
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

	update_projection_view_transform();

}

void Camera::set_lookat(vec3 from, vec3 to, vec3 up)
{
	view_transform = glm::lookAt(
		from,
		to,
		up
	);

	update_projection_view_transform();
}

void Camera::set_position(vec3 position)
{
	//Get the vector with the positions and sets it 
	world_transform[3] = glm::vec4(position, 1);
	//Recalculates the view transform based off of the world_transform 
	view_transform = glm::inverse(world_transform);
	// Update the PV transform
	update_projection_view_transform();
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

void Camera::update_projection_view_transform()
{
	projection_view_transform = projection_transform * view_transform;
}