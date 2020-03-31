#include "glm/glm.hpp"
#include "PhysicsObject.h"
#include "collision_manager.h"
#include <vector>

#include <functional>
using collisionFunction = std::function<glm::vec2(const PhysicsObject*, const PhysicsObject*)>;

#pragma once
#ifndef _PHYSICS_SCENE_H_
#define _PHYSICS_SCENE_H_

class RigidBody;

class PhysicsObject;
//void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
//{
//	applyForce(gravity * m_mass * timeStep);
//	m_position += m_velocity * timeStep;
//}
//
//void RigidBody::applyForce(glm::vec2 force)
//{
//	m_velocity = force / m_mass;
//}
//
//void RigidBody::applyForceToObject(RigidBody* object2, glm::vec2 force)
//{
//	applyForce(force);
//	object2->applyForce(-force);
//}
class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();


	void addObject(PhysicsObject* object);
	void removeObject(PhysicsObject* object);
	void update(float deltaTime);
	void updateGizmos();

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

protected:
	float m_timeStep;
	glm::vec2 m_gravity;
	std::vector<PhysicsObject*> m_objects;

	float accumulatedTime = 0.0f;

	// function pointer array for doing our collisions
	static const collisionFunction collisionFunctions[];
};


#endif // !_PHYSICS_SCENE_H_