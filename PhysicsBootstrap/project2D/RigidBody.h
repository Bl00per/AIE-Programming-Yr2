#include "PhysicsObject.h"

#pragma once
#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

class RigidBody : public PhysicsObject
{
public:
	~RigidBody() {};

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual bool checkCollision(PhysicsObject* pOther) = 0;
	virtual void debug() {}

	void applyForce(glm::vec2 force);
	void applySeparationForce(RigidBody* object2, glm::vec2 force);


	glm::vec2 getPosition() const { return m_position; }
	glm::vec2 getVelocity() const { return m_velocity; }
	float getMass() const { return m_mass; }

protected:
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float mass);

	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
};

#endif // !_RIGIDBODY_H_