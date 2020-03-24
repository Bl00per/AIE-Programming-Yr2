#include "glm/glm.hpp"

#pragma once
#ifndef _PHYSICS_OBJECT_H_
#define _PHYSICS_OBJECT_H_

enum class ShapeType : char
{
	CIRCLE = 0,
	AABB,
	LINE
};

class PhysicsObject
{
protected:
	ShapeType m_shapeID;
	PhysicsObject(ShapeType a_shapeID)
	{
		m_shapeID = a_shapeID;
	}

public:
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};
};

#endif // !_PHYSICS_OBJECT_H_