#include "PhysicsObject.h"

#pragma once
#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

class RigidBody : public PhysicsObject
{
public:
	//RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float mass);
	//~RigidBody();

	//virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	//virtual void debug();
	//void applyForce(glm::vec2 force);
	//void applyForceToObject(RigidBody* object2, glm::vec2 force);

	//virtual bool checkCollision(PhysicsObject* pOther) = 0;

	//glm::vec2 getPosition() { return m_position; }
	//glm::vec2 getVelocity() { return m_velocity; }
	//float getMass() { return m_mass; }

	glm::vec2 m_position;
	// NO ROTATION!! BIG DIFFICULT
	glm::vec2 m_velocity;
	float m_mass;

protected:
	RigidBody(ShapeType a_shapeType, const glm::vec2& a_position)
		: m_position(a_position), PhysicsObject(a_shapeType) {}
};

#endif // !_RIGIDBODY_H_