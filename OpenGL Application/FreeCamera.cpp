#include "FreeCamera.h"

//FreeCamera::FreeCamera(
//	const vec3 a_position,
//	const vec3 front,
//	const vec3 up,
//	const float fov /*= glm::radians(45.0f)*/,
//	const float aspect_ratio /*= 16.0 / 9.0f*/,
//	const float near/* = 0.1f*/,
//	const float far/* = 100.0f*/)
//{
//
//}

void FreeCamera::update(float delta_time)
{
	auto window = glfwGetCurrentContext();
	// Vector to store all the keyboard input to
	glm::vec4 input(0.0f);

	// Get the input and copy to vector (result is 1 if button is down)
	// Left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		input.x -= speed;
	// Right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	input.x += speed;
	// Forwards
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		input.z += speed;
	// Backwards
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		input.z -= speed;
	// Down
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		input.y += speed;
	// Up
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		input.y -= speed;

	// Modify the position based off of the input and the new_speed
	//this->set_position(this->get_world_transform()[3] - (input * speed * delta_time));

	glm::vec4 move_direction = glm::normalize(-input.z * world_transform[2] + input.x * world_transform[0] + input.y * world_transform[1]);

	world_transform[3] += (move_direction * speed * delta_time);
	view_transform = glm::inverse(world_transform);

	/* MOUSE LOOK */
	double cursor_position_x;
	double cursor_position_y;

	glfwGetCursorPos(window, &cursor_position_x, &cursor_position_y);
	// Calculate the offset from the screens centre this frame
	double delta_x = cursor_position_x - (1280 * 0.5);
	double delta_y = cursor_position_y - (720 * 0.5);
	// Move the cursor back to the centre of the render / window
	// TODO glfwSetInputMode(window , glfw_cursor_disabled)
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, 1280 * 0.5, 720 * 0.5);

	if (delta_x || delta_y)
	{
		// Identity matrix to accumulate rotation
		auto rotation = glm::mat4(1.0f);
		// Left / Right rotation
		rotation = glm::rotate(rotation, float(rotation_speed * delta_time * -delta_x), glm::vec3(view_transform[1]));

		// Up / Down rotation
		rotation = glm::rotate(rotation, float(rotation_speed * delta_time * -delta_y), glm::vec3(1.0f, 0.0f, 0.0f));
		// Apply the rotation to the camera
		world_transform = world_transform * rotation;
		// Update PxV
		view_transform = glm::inverse(world_transform);
	}


	update_projection_view_transform();
}

void FreeCamera::set_speed(float a_speed)
{
	speed = a_speed;
}