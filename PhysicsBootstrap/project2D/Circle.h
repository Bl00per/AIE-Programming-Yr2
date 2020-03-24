#pragma once
#include "RigidBody.h"

class circle : public RigidBody
{
public:

	circle(glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_radius, const glm::vec4 a_colour);
	~circle();
	//circle(const float a_radius, const glm::vec2& a_position = glm::vec2(0.0f));

	const float getRadius() const;
	
	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);
	virtual void debug() {}

	void setRadius(const float a_radius);
	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }

protected:
	float m_radius;
	glm::vec4 m_colour;
};


