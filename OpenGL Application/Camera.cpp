/* camera.cpp
   @brief Camera function implementation

   Adam Clarke - 21/04/2017

   Edit - 2018 05 24 - Updated function definitions to reflect updated declarations.
					 - Updated getters to new glm with implicity vec3(vec4) casting
		- 2019 08 29 - Ported to gl_foundation_x64 as tutorial code
*/
#include "camera.h"

/* @namespace MilkGL namespace
*/
namespace slab
{
	/* @brief Set a new perspective for the camera
	   @param Field of view in Y screen space around local X axis
	   @param Cameras's aspect ratio
	   @param Near plane distance in world units
	   @param Far plane distance in world units
	*/
	void camera::set_perspective(const float a_FOV_Y, const float a_aspect_ratio,
		const float a_near_distance, const float a_far_distance)
	{
		aspect_ratio = a_aspect_ratio;
		projection_matrix = glm::perspective(a_FOV_Y, aspect_ratio, a_near_distance, a_far_distance);
		update_matricies();
	}

	/* @brief Set a new orthographic projection
	   @param Width in world units
	   @param Height in world units
	   @param Near plane depth
	   @param Far plane depth
	*/
	void camera::set_orthographic(const float a_width, const float a_height,
		const float a_near_distance, const float a_far_distance)
	{
		aspect_ratio = a_width / a_height;
		projection_matrix = glm::ortho(-a_width * 0.5f, a_width * 0.5f, -a_height * 0.5f, a_height * 0.5f, a_near_distance, a_far_distance);
		update_matricies();
	}

	/* @brief Set look from and to points
	   @param Camera's position
	   @param Camera's look-target position
	   @param Camera;s yaw axis (Camera Up) Y axis by default
	*/
	void camera::set_look_at(const glm::vec3 &a_from_position, const glm::vec3 &a_to_position,
		const glm::vec3 &a_yaw_axis)
	{
		view_matrix = glm::lookAt(a_from_position, a_to_position, a_yaw_axis);
		world_matrix = glm::inverse(view_matrix);
		update_matricies();
	}

	/* @brief Protected constructor for matrix default initialization
	*/
	camera::camera()
	{
		world_matrix = glm::mat4(1.0f);
		view_matrix = glm::mat4(1.0f);
		projection_matrix = glm::mat4(1.0f);
		projection_x_view_matrix = glm::mat4(1.0f);
		aspect_ratio = 1.0f;
	}

	/* @brief Update whever the projection of view change
	*/
	void camera::update_matricies()
	{
		projection_x_view_matrix = projection_matrix * view_matrix;
	}

	/* @brief Move the camera to this position
	   @param Camera's new position
	*/
	void camera::set_position(const glm::vec3 &a_position)
	{
		world_matrix[3] = glm::vec4(a_position, 1);
		view_matrix = glm::inverse(world_matrix);
		update_matricies();
	}

	/* @brief Returns camera current position
	   @param Copy of camera's position
	*/
	glm::vec3 camera::get_position() const
	{
		return glm::vec3(world_matrix[3]);
	}

	/* @brief Returns camera current forward direction
	   @param Copy of camera's current forward direction
	*/
	glm::vec3 camera::get_direction() const
	{
		return glm::vec3(world_matrix[2]);
	}

	/* @brief Returns camera current up direction
	   @param Copy of camera's current up direction
	*/
	glm::vec3 camera::get_up_vector() const
	{
		return glm::vec3(world_matrix[1]);
	}

	/* @brief Read camera's world transform
	   @return Copy of camera's world transform
	*/
	glm::mat4 camera::get_world_transform() const
	{
		return world_matrix;
	}

	/* @brief Read camera's view transform
		@return Copy of camera's world transform
	*/
	glm::mat4 camera::get_view_transform() const
	{
		return view_matrix;
	}

	/* @brief Read Camera's projection transform
	   @return Copy of camera's projection transform
	*/
	glm::mat4 camera::get_projection_transform() const
	{
		return projection_matrix;
	}

	/* @brief  Get a precalculated Projection x View transform
	   @return Copy of PxV transforms
	*/
	glm::mat4 camera::get_projection_view_transform() const
	{
		return projection_x_view_matrix;
	}

}