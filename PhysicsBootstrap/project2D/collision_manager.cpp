#include "collision_manager.h"
#include <cmath>

glm::vec2 collision_manager::circle_vs_circle(const PhysicsObject* a_circle_A, const PhysicsObject* a_circle_B)
{
	const circle* c1 = dynamic_cast<const circle*>(a_circle_A);
	const circle* c2 = dynamic_cast<const circle*>(a_circle_B);

	assert(c1 && c2 && " Circle1 or Circle2 was null in collision check");

	glm::vec2 displacement = (c1->getPosition() - c2->getPosition());
	float sum_of_radii = c1->getRadius() + c2->getRadius();
	float overlap = abs(sum_of_radii) - glm::length(displacement);

	if (overlap > 0.0f)
	{
		return glm::normalize(displacement) * overlap;
	}

	return glm::vec2(0.0f);
}

glm::vec2 collision_manager::aabb_vs_aabb(const PhysicsObject* a_aabb_A, const PhysicsObject* a_aabb_B)
{
	const alignedBoundingBox* aabb_A = dynamic_cast<const alignedBoundingBox*>(a_aabb_A);
	const alignedBoundingBox* aabb_B = dynamic_cast<const alignedBoundingBox*>(a_aabb_B);

	const float maxOverlap = 10000000.0f;
	float overlap = maxOverlap; // <- numeric float limit would be a better choice
	glm::vec2 overlapVector = glm::vec2();

	// Given A and B and aabbs, lets check each axis for an overlap
	// X AXIS CHECK
	if (aabb_A->getMax().x > aabb_B->getMin().x)
	{
		float localOverlap = abs(aabb_A->getMax().x - aabb_B->getMin().x);
		if (localOverlap < overlap)
		{
			overlap = localOverlap;
			overlapVector = glm::vec2(-overlap, 0.0f);
		}
	}
	else
		return glm::vec2(0.0f);

	// Check the other side
	if (aabb_A->getMin().x > aabb_B->getMax().x)
	{
		float localOverlap = abs(aabb_A->getMin().x - aabb_B->getMax().x);
		if (localOverlap < overlap)
		{
			overlap = localOverlap;
			// Other direction along x
			overlapVector = glm::vec2(overlap, 0.0f);
		}
	}
	else
		return glm::vec2(0.0f);

	// Y AXIS CHECK
	if (aabb_A->getMax().y > aabb_B->getMin().y)
	{
		float localOverlap = abs(aabb_A->getMax().y - aabb_B->getMin().y);
		if (localOverlap < overlap)
		{
			overlap = localOverlap;
			overlapVector = glm::vec2(0.0f, overlap);
		}
	}
	else
		return glm::vec2(0.0f);

	// Check the other side
	if (aabb_A->getMin().y > aabb_B->getMax().y)
	{
		float localOverlap = abs(aabb_A->getMin().y - aabb_B->getMax().y);
		if (localOverlap < overlap)
		{
			overlap = localOverlap;
			// Other direction along x
			overlapVector = glm::vec2(0.0f, -overlap);
		}
	}
	else
		return glm::vec2(0.0f);

	return overlapVector;
}

glm::vec2 collision_manager::line_vs_line(const PhysicsObject* a_line_A, const PhysicsObject* a_line_B)
{
	return glm::vec2(0.0f);
}

glm::vec2 collision_manager::aabb_vs_circle(const PhysicsObject* a_aabb, const PhysicsObject* a_circle)
{
	const alignedBoundingBox* aabb = dynamic_cast<const alignedBoundingBox*>(a_aabb);
	const circle* c = dynamic_cast<const circle*>(a_circle);

	glm::vec2 clamped_centre = glm::clamp(c->getPosition(), aabb->getMin(), aabb->getMax());
	glm::vec2 displacement = clamped_centre - c->getPosition();
	float overlap = c->getRadius() - glm::length(displacement);
	if (overlap > 0.0f)
	{
		return glm::normalize(displacement) * overlap;
	}

	return glm::vec2(0.0f);
}

glm::vec2 collision_manager::circle_vs_aabb(const PhysicsObject* a_circle, const PhysicsObject* a_aabb)
{
	return aabb_vs_circle(a_aabb, a_circle);
}

glm::vec2 collision_manager::circle_vs_line(const PhysicsObject* a_circle, const PhysicsObject* a_line)
{
	const circle* c = dynamic_cast<const circle*>(a_circle);
	const line* l = dynamic_cast<const line*>(a_line);

	assert(c && l && " Circle or Line was null in collision check");

	float position_dot_normal = glm::dot(c->getPosition(), l->getNormal());

	float distance = position_dot_normal - (l->getDistance() + c->getRadius());

	if (distance < 0.0f)
	{
		return l->getNormal() * -distance;
	}
	else
		return glm::vec2(0.0f);
}

glm::vec2 collision_manager::line_vs_circle(const PhysicsObject* a_line, const PhysicsObject* a_circle)
{
	return circle_vs_line(a_circle, a_line);
}

glm::vec2 collision_manager::aabb_vs_line(const PhysicsObject* a_aabb, const PhysicsObject* a_line)
{
	//	 #1 loop each corner and dot product the distance
	//		then the furthest negative distance is the corner of greatest overlap
	// float position_dot_normal =
	//	a_aabb.getMax().dot(a_line.get_normal());
	// float distance = position_dot_normal - a_line.get_distance();
	const alignedBoundingBox* aabb = dynamic_cast<const alignedBoundingBox*>(a_aabb);
	const line* l = dynamic_cast<const line*>(a_line);

	assert(aabb && l && " AABB or Line was null in collision check");

	glm::vec2 extents = 0.5f * aabb->getExtents();
	glm::vec2 extents_neg_x = 0.5f * glm::vec2(-aabb->getExtents().x, aabb->getExtents().y);

	float dot_extents = glm::dot(extents, (l->getNormal()));
	float dot_extents_neg_x = glm::dot(extents_neg_x, l->getNormal());
	float radius = fmaxf(fabsf(dot_extents), fabsf(dot_extents_neg_x));

	circle projection(aabb->getPosition(), glm::vec2(0.0f), aabb->getMass(), radius/*, glm::vec4(0.0f)*/);

	return circle_vs_line(&projection, a_line);
}

glm::vec2 collision_manager::line_vs_aabb(const PhysicsObject* a_line, const PhysicsObject* a_aabb)
{
	return aabb_vs_line(a_aabb, a_line);
}
