/*	free_camera.cpp
	@brief WASD based Mouselook camera implementation
	@note Needs to reach out to the GLFW context for processing input
	Adam Clarke - 2019 08 29
*/
/* Class declaration */
#include "free_camera.h"
/* OpenGL access for input */
#include "glfw3.h"

/*	@brief Constructor to position and orient a free roaming camera in the scene
		@param Position of the camera
		@param Initial target look-at location
		@param The field of view up and down along Y in radians
		@param The camera's aspect ratio
		@param Camera's near clip distance in world units
		@param Camera's far clip distance in world units
	*/
free_camera::free_camera(
	const glm::vec3 a_position, 
	const glm::vec3 a_look_at_point,
	const float a_aspect_ratio /* = 16.0 / 9.0f*/,
	const float a_FOV_Y /* = 3.14159f * 0.5f */,
	const float a_near_distance/*  = 0.5f*/,
	const float a_far_distance /*= 100.0f */) : slab::camera()
{
	set_perspective(a_FOV_Y, a_aspect_ratio, a_near_distance, a_far_distance);
	set_look_at(a_position, a_look_at_point);
}

/*	@brief Call this once per frame to allow for input processing and movement
	@param The time since last frame
*/
void free_camera::update(double a_delta_time)
{
	// Grab context
	auto glfw_window = glfwGetCurrentContext();
	// Only update camera if input is received
	bool input_flag = false;
	// The build translation vector
	glm::vec3 displacement = glm::vec3(0.0f);

	/* CLASSIC WASD */
	// Forward
	if (glfwGetKey(glfw_window, GLFW_KEY_W))
	{
		displacement -= get_direction();
		//displacement -= get_direction();
		input_flag = true;
	}
	// Backward
	if (glfwGetKey(glfw_window, GLFW_KEY_S))
	{
		displacement += get_direction();
		input_flag = true;
	}
	// Left
	if (glfwGetKey(glfw_window, GLFW_KEY_A))
	{
		displacement -= glm::vec3(world_matrix[0]);
		input_flag = true;
	}
	// Right
	if (glfwGetKey(glfw_window, GLFW_KEY_D))
	{
		displacement += glm::vec3(world_matrix[0]);
		input_flag = true;
	}
	// Up
	if (glfwGetKey(glfw_window, GLFW_KEY_LEFT_CONTROL))
	{
		displacement -= glm::vec3(0.0f, 1.0f, 0.0f);
		input_flag = true;
	}
	// Down
	if (glfwGetKey(glfw_window, GLFW_KEY_SPACE))
	{
		displacement += glm::vec3(0.0f, 1.0f, 0.0f);
		input_flag = true;
	}
	
	// Apply the scaled translation to the camera's position, if there was input
	if (input_flag)
	{
		set_position(glm::vec3(world_matrix[3] + (glm::vec4(displacement, 0.0f) * translation_speed * a_delta_time)));
		update_matricies();
	}

	/* MOUSE LOOK */
	double cursor_position_x;
	double cursor_position_y;
	// Aquire the current cursor position
	glfwGetCursorPos(glfw_window, &cursor_position_x, &cursor_position_y);

	// TODO - STORE Resolution of the camera
	
	// Calculate the offset from the screens centre this frame
	double delta_x = cursor_position_x - (1280 * 0.5);
	double delta_y = cursor_position_y - (720 * 0.5);
	// Move the cursor back to the centre of the render / window
	// TODO glfwSetInputMode(window , glfw_cursor_disabled)
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(glfw_window, 1280 * 0.5, 720 * 0.5);

	// If either input is non-zero, apply the rotation
	if (delta_x || delta_y)
	{	
		// Identity matrix to accumulate rotation
		auto rotation = glm::mat4(1.0f); 
		// Left / Right rotation
		rotation = rotation * glm::rotate(float(angular_speed * a_delta_time * delta_x), glm::vec3(view_matrix[1]));
		// Up / Down rotation
		rotation = rotation * glm::rotate(float(angular_speed * a_delta_time * -delta_y), glm::vec3(1.0f, 0.0f, 0.0f));
		
		// Apply the rotation to the camera
		world_matrix = world_matrix * rotation;
		// Preserve the inverse
		view_matrix = glm::inverse(world_matrix);
		// Update PxV
		update_matricies();
	}

}