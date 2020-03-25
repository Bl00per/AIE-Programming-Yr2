#pragma once
#include "RigidBody.h"

class alignedBoundingBox : public RigidBody
{
public:
	alignedBoundingBox(const glm::vec2 a_position, glm::vec2 a_velocity, 
		float a_mass, const glm::vec2 a_extents, glm::vec4 a_colour);
	~alignedBoundingBox();

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);
	virtual void debug() {}

	const glm::vec2 getPosition() const;
	void setPosition(const glm::vec2& a_position);

	const glm::vec2 getExtents() const;
	void setExtents(const glm::vec2& a_extents);

	const float getWidth() const;
	const float getWeight() const;

	const glm::vec2 getMin() const;
	const glm::vec2 getMax() const;

	glm::vec4 getColour() { return m_colour; }

protected:
	glm::vec4 m_colour;

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

