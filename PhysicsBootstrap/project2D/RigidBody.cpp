#include "RigidBody.h"

void RigidBody::fixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
	applyForce(a_gravity * m_mass * a_timeStep);
	m_velocity -= m_velocity * drag * a_timeStep;
	m_position += m_velocity * a_timeStep;
}

void RigidBody::applyForce(glm::vec2 a_force)
{
	m_velocity += a_force / m_mass; // A = f / m
}

void RigidBody::applySeparationForce(RigidBody* a_object, glm::vec2 a_force)
{
	a_object->applyForce(a_force);
	this->applyForce(-a_force);
}

//RigidBody(ShapeType a_shapeType, const glm::vec2& a_position)
//	: m_position(a_position), PhysicsObject(a_shapeType) {}

RigidBody::RigidBody(ShapeType a_shapeID, glm::vec2 a_position, glm::vec2 a_velocity, float a_mass) : 
	PhysicsObject(a_shapeID),
	m_position(a_position),
	m_velocity(a_velocity),
	m_mass(a_mass)
{}

void RigidBody::resolveCollision(RigidBody* a_other, glm::vec2 a_collisionNormal)
{
	glm::vec2 normal = glm::normalize(a_collisionNormal);

	glm::vec2 relativeVelocity = a_other->getVelocity() - m_velocity;

	float j = glm::dot(-(1 + (elasticity * a_other->elasticity)) * (relativeVelocity), normal) /
		glm::dot(normal, normal * ((1 / m_mass) + (1 / a_other->getMass())));

	glm::vec2 force = normal * j;

	applySeparationForce(a_other, force);
}
