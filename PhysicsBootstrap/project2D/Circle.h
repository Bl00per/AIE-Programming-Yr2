#include "RigidBody.h"
#include "Gizmos.h"

#pragma once
#ifndef _CIRCLE_H_
#define _CIRCLE_H_

class Circle : public RigidBody
{
public:
	Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);
	~Circle();

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);

	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }

protected:
	float m_radius;
	glm::vec4 m_colour;
};

#endif // !_CIRCLE_H_