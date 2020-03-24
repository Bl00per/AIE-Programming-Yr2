#include "circle.h"
#include "collision_manager.h"
#include <cmath>

//circle::circle(const float a_radius, const glm::vec2& a_position) : 
//	RigidBody(ShapeType::CIRCLE, a_position), 
//	m_radius(a_radius)
//{}


circle::circle(glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_radius, const glm::vec4 a_colour) :
	RigidBody(ShapeType::CIRCLE, a_position, a_velocity, a_mass),
	m_radius(a_radius),
	m_colour(a_colour)
{}

circle::~circle()
{}

void circle::setRadius(const float a_radius)
{
	m_radius = abs(a_radius);
}

const float circle::getRadius() const
{
	return m_radius;
}

void circle::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 32, m_colour);
}

bool circle::checkCollision(PhysicsObject* pOther)
{
	return collision_manager::circle_vs_circle(*this, (circle&)*pOther);
}