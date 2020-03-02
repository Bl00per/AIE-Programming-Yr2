/* camera.h
   @brief Base camera class for OpenGL.
   Stores all the transformation matricies
   to position, move and view from the camera in the scene.

   Adam Clarke - 21/04/2017

   Edit - 2018 05 24 - Removed GLFWwindow reference in update. This was
					   legacy tutorial code for input polling to move the camera.
		- 2019 08 29 - Ported to gl_foundation_x64 as tutorial code
*/
#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_

/* Maths headers for transforms and cameras */
#include <glm.hpp>
#include <ext.hpp>

/* @namespace slab
*/
namespace slab
{
	/* @brief Definition of World, View, and Projection transforms, and
	   access and modifier functions.
	*/
	class camera
	{
	public:
		/* @brief Pure virtual update as each camera will have a unique update
		   @param Frame time if needed for moving
		*/
		virtual void update(double a_delta_time) = 0;

		/* @brief Set a new perspective for the camera
		   @param Field of view in Y screen space around local X axis
		   @param Cameras's aspect ratio
		   @param Near plane distance in world units
		   @param Far plane distance in world units
		*/
		void set_perspective(const float a_FOV_Y, const float a_aspect_ratio,
			const float a_near_distance, const float a_far_distance);

		/* @brief Set a new orthographic projection
		   @param Width in world units
		   @param Height in world units
		   @param Near plane depth
		   @param Far plane depth
		*/
		void set_orthographic(const float a_width, const float a_height,
			const float a_near_distance, const float a_far_distance);

		/* @brief Set look from and to points
		   @param Camera's position
		   @param Camera's look-target position
		   @param Camera;s yaw axis (Camera Up) Y axis by default
		*/
		void set_look_at(const glm::vec3 &a_from_position, const glm::vec3 &a_to_position,
			const glm::vec3 &a_yaw_axis = glm::vec3(0.0f, 1.0f, 0.0f));

		/* @brief Move the camera to this position
		   @param Camera's new position
		*/
		void set_position(const glm::vec3 &a_position);

		/* @brief Returns camera current position
		   @param Copy of camera's position
		*/
		glm::vec3 get_position() const;

		/* @brief Returns camera current forward direction
		   @param Copy of camera's current forward direction
		*/
		glm::vec3 get_direction() const;

		/* @brief Returns camera current up direction
		   @param Copy of camera's current up direction
		*/
		glm::vec3 get_up_vector() const;

		/* @brief Read camera's world transform
		   @return Copy of camera's world transform
		*/
		glm::mat4 get_world_transform() const;

		/* @brief Read camera's view transform
		   @return Copy of camera's world transform
		*/
		glm::mat4 get_view_transform() const;

		/* @brief Read Camera's projection transform
		   @return Copy of camera's projection transform
		*/
		glm::mat4 get_projection_transform() const;

		/* @brief  Get a precalculated Projection x View transform
		   @return Copy of PxV transforms
		*/
		glm::mat4 get_projection_view_transform() const;

	protected:
		/*	@brief Protected constructor for matrix default initialization
		*/
		camera();

		/*	@brief Camera position and rotation
		*/
		glm::mat4 world_matrix;

		/*	@brief Inverse world transform for rendering
		*/
		glm::mat4 view_matrix;

		/*	@brief Projection matrix information
		*/
		glm::mat4 projection_matrix;

		/*	@brief Projection x View transformations
		*/
		glm::mat4 projection_x_view_matrix;

		/*	@brief Aspect ratio for given projection
		*/
		float aspect_ratio;

		/*	@brief Update whever the projection of view change
		*/
		void update_matricies();
	};
}
#endif // _CAMERA_H_

