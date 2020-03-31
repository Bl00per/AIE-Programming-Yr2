#pragma once
#include "PhysicsObject.h"
#include "glm/glm.hpp"

class RigidBody;

class line : public PhysicsObject
{
public:
	line(const glm::vec2& a_normal,
		const float a_distance = 0.0f);

	const glm::vec2 getNormal() const;
	void setNormal(const glm::vec2& a_normal);

	const float getDistance() const;
	void setDistance(const float a_distance);

	virtual void debug() {}
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) {};
	virtual void makeGizmo();

	void resolveCollision(RigidBody* a_other);

private:
	glm::vec2 m_normal = glm::vec2(0, 1);
	float m_distance_from_origin = 0.0f;
};

