#pragma once
#include "RigidBody.h"

class circle : public RigidBody
{
public:

	circle(const float a_radius, const glm::vec2& a_position = glm::vec2(0.0f));

	const float getRadius() const;
	
	/*	@brief Sets the radius to the absolute value of the argument
		@param The new radius as float
	*/
	void setRadius(const float a_radius);

	const glm::vec2 getPosition() const;
	void setPosition(const glm::vec2& a_position);

private:
	float m_radius = 0.0f;
};


