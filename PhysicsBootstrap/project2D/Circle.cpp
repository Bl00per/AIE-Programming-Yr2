#include "Circle.h"

Circle::Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) : 
	RigidBody(CIRCLE, position, velocity, mass)
{
	m_radius = radius;
	m_colour = colour;
}

void Circle::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, m_mass, m_colour);
}

bool Circle::checkCollision(PhysicsObject* pOther)
{
	
}
