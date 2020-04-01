#include "line.h"
#include "RigidBody.h"
#include <limits>

line::line(const glm::vec2& a_normal /* = { 0.0f, 1.0f }*/,
			const float a_distance /*= 0.0f*/) :
	m_normal(a_normal),
	m_distance_from_origin(a_distance),
	PhysicsObject(ShapeType::LINE)
{
	setNormal(m_normal);
}

const glm::vec2 line::getNormal() const
{
	return m_normal;
}

void line::setNormal(const glm::vec2& a_normal)
{
	if (glm::length(a_normal) <= 0.0f)
	{
	}
	else
	{
		m_normal = glm::normalize(a_normal);
	}
}

const float line::getDistance() const
{
	return m_distance_from_origin;
}

void line::setDistance(const float a_distance)
{
	m_distance_from_origin = a_distance;
}

void line::makeGizmo()
{
	glm::vec2 centrePoint = m_normal * m_distance_from_origin;
	glm::vec2 drawDirection(m_normal.y, -m_normal.x);
	aie::Gizmos::add2DLine(centrePoint + (drawDirection * 500.0f),
						   centrePoint - (drawDirection * 500.0f),
						   glm::vec4(1.0f));
}

void line::resolveCollision(RigidBody* a_other)
{
	glm::vec2 relativeVelocity = a_other->getVelocity();

	float j = glm::dot(-(1 + (elasticity * a_other->elasticity)) * (relativeVelocity), m_normal) /
		glm::dot(m_normal, m_normal * (1 / a_other->getMass()));

	glm::vec2 force = m_normal * j;

	a_other->applyForce(force);
}