#include "circle.h"
#include <cmath>

circle::circle(const float a_radius, const glm::vec2& a_position) : 
	RigidBody(ShapeType::CIRCLE, a_position), 
	m_radius(a_radius)
{}

const float circle::getRadius() const
{
	return m_radius;
}
void circle::setRadius(const float a_radius)
{
	m_radius = abs(a_radius);
}

const glm::vec2 circle::getPosition() const
{
	return m_position;
}

void circle::setPosition(const glm::vec2& a_position)
{
	m_position = a_position;
}

