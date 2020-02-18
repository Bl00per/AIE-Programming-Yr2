#pragma once
#include "glfw3.h"
#include "glm.hpp"
#include "ext.hpp"

using mat4 = glm::mat4;
using vec3 = glm::vec3;

enum class camera_movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = -2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

class Camera
{
public:
	// Camera Attributes
	vec3 position;
	vec3 front;
	vec3 up;
	vec3 right;
	vec3 world_up;
	// Euler Angles
	float yaw;
	float pitch;
	// Camera Options
	float movement_speed;
	float mouse_sensitivity;
	float fov;

	// Constructor with vectors
	Camera(vec3 a_position, vec3 a_up, float a_yaw, float a_pitch) :
		front(vec3(0.0f, 0.0f, -1.0f)),
		movement_speed(SPEED),
		mouse_sensitivity(SENSITIVITY),
		fov(FOV)
	{
		position = a_position;
		world_up = a_up;
		yaw = a_yaw;
		pitch = a_pitch;

		world_transform = mat4(1.0f);
		view_transform = mat4(1.0f);
		projection_transform = mat4(1.0f);
		projection_view_transform = mat4(1.0f);

		set_perspective(fov, 16.0 / 9.0f, 0.1f, 100.0f);
		set_lookat(vec3(3.0f, 0.0f, 20.0f), vec3(-3.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

		update_camera_vectors();
		update_projection_view_transform();
	}

	// Constructor with scalar values
	Camera(float a_pos_x, float a_pos_y, float a_pos_z, float a_up_x, float a_up_y, float a_up_z, float a_yaw, float a_pitch) :
		front(vec3(0.0f, 0.0f, -1.0f)),
		movement_speed(SPEED),
		mouse_sensitivity(SENSITIVITY),
		fov(FOV)
	{
		position = vec3(a_pos_x, a_pos_y, a_pos_z);
		world_up = vec3(a_up_x, a_up_y, a_up_z);
		yaw = a_yaw;
		pitch = a_pitch;

		update_camera_vectors();
	}

	virtual void update(float a_delta_time);

	void process_keyboard(camera_movement direction, float a_delta_time);
	void process_mouse_movement(float x_offset, float y_offset, GLboolean constrain_pitch = true);
	void process_mouse_scroll(float y_offset);

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

protected:
	mat4 world_transform;
	mat4 view_transform;
	mat4 projection_transform;
	mat4 projection_view_transform;

	void update_projection_view_transform();

private:
	void update_camera_vectors();

	float a_delta_time;
	float last_frame;
};

