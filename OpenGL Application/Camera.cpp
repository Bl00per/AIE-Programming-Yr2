#include "Camera.h"

//Camera::Camera(
//	vec3 a_position,
//	vec3 a_up,
//	float a_yaw,
//	float a_pitch) :
//	front(vec3(0.0f, 0.0f, -1.0f)),
//	movement_speed(SPEED),
//	mouse_sensitivity(SENSITIVITY),
//	fov(FOV)
//{
//	position = a_position;
//	world_up = a_up;
//	yaw = a_yaw;
//	pitch = a_pitch;
//
//	world_transform = mat4(1.0f);
//	view_transform = mat4(1.0f);
//	projection_transform = mat4(1.0f);
//	projection_view_transform = mat4(1.0f);
//
//	set_perspective(fov, 16.0 / 9.0f, 0.1f, 100.0f);
//	set_lookat(vec3(3.0f, 0.0f, 20.0f), vec3(-3.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
//
//	update_camera_vectors();
//	update_projection_view_transform();
//}
//
//Camera::Camera(float a_pos_x, float a_pos_y, float a_pos_z, float a_up_x, float a_up_y, float a_up_z, float a_yaw, float a_pitch) :
//	front(vec3(0.0f, 0.0f, -1.0f)),
//	movement_speed(SPEED),
//	mouse_sensitivity(SENSITIVITY),
//	fov(FOV)
//{
//	position = vec3(a_pos_x, a_pos_y, a_pos_z);
//	world_up = vec3(a_up_x, a_up_y, a_up_z);
//	yaw = a_yaw;
//	pitch = a_pitch;
//
//	update_camera_vectors();
//}

void Camera::update(float a_delta_time)
{
}

void Camera::process_keyboard(camera_movement direction, float a_delta_time)
{
	float velocity = movement_speed * a_delta_time;
	glm::vec4 position = world_transform[3];
	if (direction == camera_movement::FORWARD)
		position -= world_transform[2] * velocity;
	if (direction == camera_movement::BACKWARD)
		position += world_transform[2] * velocity;
	if (direction == camera_movement::LEFT)
		position -= world_transform[0] * velocity;
	if (direction == camera_movement::RIGHT)
		position += world_transform[0] * velocity;
	if (direction == camera_movement::UP)
		position += world_transform[1] * velocity;
	if (direction == camera_movement::DOWN)
		position -= world_transform[1] * velocity;

	set_position(glm::vec3(position));
}

void Camera::process_mouse_movement(float x_offset, float y_offset, GLboolean constrain_pitch)
{
	x_offset *= mouse_sensitivity;
	y_offset *= mouse_sensitivity;

	yaw += x_offset;
	pitch += y_offset;

	// Make sure that when the pitch is out of bounds, the screen doesn't get flipped
	if (constrain_pitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	update_camera_vectors();
}

void Camera::process_mouse_scroll(float y_offset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= y_offset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}



void Camera::set_perspective(float fov, float aspect_ratio, float near, float far)
{
	projection_transform = glm::perspective(
		fov,
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
	world_transform = glm::inverse(view_transform);

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
	return glm::lookAt(position, position + front, up);
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

void Camera::update_camera_vectors()
{
	vec3 m_front;
	m_front.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
	m_front.y = sin(glm::radians(pitch));
	m_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(m_front);
	right = glm::normalize(glm::cross(front, world_up));
	up = glm::normalize(glm::cross(right, front));
}