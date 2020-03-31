#include "glm/glm.hpp"
#include "Gizmos.h"

#pragma once
#ifndef _PHYSICS_OBJECT_H_
#define _PHYSICS_OBJECT_H_

enum class ShapeType : char
{
	CIRCLE = 0,
	AABB,
	LINE,
	SHAPECOUNT
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

	void setColour(glm::vec4 a_color)
	{
		color = a_color;
	}
	glm::vec4 color = glm::vec4(1.0f);

	ShapeType getShapeID() const
	{
		return m_shapeID;
	}
};

#endif // !_PHYSICS_OBJECT_H_