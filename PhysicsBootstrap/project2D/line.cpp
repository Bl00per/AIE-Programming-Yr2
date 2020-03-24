#include "line.h"
#include <limits>

line::line(const glm::vec2& a_normal /* = { 0.0f, 1.0f }*/,
			const float a_distance /*= 0.0f*/) :
	m_normal(a_normal),
	m_distance_from_origin(a_distance),
	PhysicsObject(ShapeType::LINE)
{}

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