#pragma once
#include "RigidBody.h"

class alignedBoundingBox : public RigidBody
{
public:
	alignedBoundingBox(const glm::vec2& a_position,
						 const glm::vec2& a_extents);

	const glm::vec2 getPosition() const;
	void setPosition(const glm::vec2& a_position);

	const glm::vec2 getExtents() const;
	void setExtents(const glm::vec2& a_extents);

	const float getWidth() const;
	const float getWeight() const;

	const glm::vec2 getMin() const;
	const glm::vec2 getMax() const;

private:
	union
	{
		glm::vec2 m_extents;
		struct
		{
			float m_width;
			float m_height;
		};
	};
};

