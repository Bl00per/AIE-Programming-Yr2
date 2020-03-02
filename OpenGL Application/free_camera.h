/*	free_camera.h
	@brief WASD based Mouselook camera for scene roaming
	Adam Clarke - 2019 08 29
*/
#pragma once
#ifndef _FREE_CAMERA_H_
#define _FREE_CAMERA_H_

/* Base class header */
#include "camera.h"

/*	@brief WASD based Mouselook camera implemetation
*/
class free_camera : public slab::camera
{
public:
	/*	@brief Constructor to position and orient a free roaming camera in the scene
		@param Position of the camera
		@param Initial target look-at location
		@param The field of view up and down along Y in radians
		@param The camera's aspect ratio
		@param Camera's near clip distance in world units
		@param Camera's far clip distance in world units
	*/
	free_camera(
		const glm::vec3 a_position, 
		const glm::vec3 a_look_at_point,
		const float a_aspect_ratio = 16.0/9.0f,
		const float a_FOV_Y = 3.14159f * 0.5f, // 90 on Y, roughly 120 on X
		const float a_near_distance = 0.01f, 
		const float a_far_distance = 10.0f);

	/*	@brief Call this once per frame to allow for input processing and movement
		@param The time since last frame
	*/
	void update(double a_delta_time) override;

	/*	@brief Setter for the cameras movement speed in gl_units per second
		@param New speed that the camera will move around the space
	*/
	void set_translation_speed(float a_translation_speed);

	/*	@brief 
		@param 
	*/
	auto get_translation_speed() const;

	/*	@brief Setter for the cameras angular speed ratio, 
		@note The speed itself is determined by mouse motion 
		@param New angular speed ratio
	*/
	void set_angular_speed(float a_angular_speed);

	/*	@brief
		@param
	*/
	[[nodiscard]] constexpr auto get_angular_speed() const noexcept
	{
		return angular_speed;
	}
private:
	
	/*	@brief The movement speed of the camera in world units per second */
	float translation_speed = 5.0f;
	/*  @breif A weight for converting delta-mouse motion into rotation */
	float angular_speed = 0.1f;
};
#endif //!_FREE_CAMERA_H_

