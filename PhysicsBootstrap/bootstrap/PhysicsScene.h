#include "glm/glm.hpp"
#include "PhysicsObject.h"
#include <vector>

#pragma once
#ifndef _PHYSICS_SCENE_H_
#define _PHYSICS_SCENE_H_

class PhysicsObject;

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
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_objects;
};

#endif // !_PHYSICS_SCENE_H_