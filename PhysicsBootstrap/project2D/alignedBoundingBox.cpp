#include "alignedBoundingBox.h"
#include <cmath>

alignedBoundingBox::alignedBoundingBox(const glm::vec2& a_position, const glm::vec2& a_extents) :
	RigidBody(ShapeType::AABB, a_position),
	m_extents(a_extents)
{
}

const glm::vec2 alignedBoundingBox::getPosition() const
{
	return m_position;
}

void alignedBoundingBox::setPosition(const glm::vec2& a_position)
{
	m_position = a_position;
}

const glm::vec2 alignedBoundingBox::getExtents() const
{
	return m_extents;
}

void alignedBoundingBox::setExtents(const glm::vec2& a_extents)
{
	m_extents = { abs(a_extents.x), abs(a_extents.y) };
}

const float alignedBoundingBox::getWidth() const
{
	return m_width;
}

const float alignedBoundingBox::getWeight() const
{
	return m_height;
}

const glm::vec2 alignedBoundingBox::getMin() const
{
	return m_position - 0.5f * m_extents;
}

const glm::vec2 alignedBoundingBox::getMax() const
{
	return m_position + 0.5f * m_extents;
}