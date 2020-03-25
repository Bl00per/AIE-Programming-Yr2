#include "collision_manager.h"
#include <cmath>

bool collision_manager::circle_vs_circle(
	const circle& a_circle_A,
	const circle& a_circle_B)
{
	glm::vec2 displacement = (a_circle_A.getPosition() - a_circle_B.getPosition());
	float sum_of_radii = a_circle_A.getRadius() + a_circle_B.getRadius();
	return (glm::length(displacement) < sum_of_radii);
}

bool collision_manager::aabb_vs_aabb(
	const alignedBoundingBox& a_aabb_A,
	const alignedBoundingBox& a_aabb_B)
{
	return !(a_aabb_A.getMax().x < a_aabb_B.getMin().x ||
			 a_aabb_A.getMin().x > a_aabb_B.getMax().x ||
			 a_aabb_A.getMax().y < a_aabb_B.getMin().y ||
			 a_aabb_A.getMin().y > a_aabb_B.getMax().y);
}

bool collision_manager::aabb_vs_circle(
	const alignedBoundingBox& a_aabb,
	const circle& a_circle)
{
	glm::vec2 clamped_centre = clamp(a_circle.getPosition(), a_aabb.getMin(), a_aabb.getMax());
	glm::vec2 displacement = clamped_centre - a_circle.getPosition();
	return (glm::length(displacement) < a_circle.getRadius());
}

//bool collision_manager::circle_vs_line(
//	const circle& a_circle,
//	const line& a_line)
//{
//	float position_dot_normal = 
//		a_circle.getPosition().dot(a_line.get_normal());
//	float distance = position_dot_normal -
//		(a_line.get_distance() + a_circle.getRadius());
//	return (distance < 0);
//}
//
//bool collision_manager::aabb_vs_line(
//	const alignedBoundingBox& a_aabb,
//	const line& a_line)
//{
//	// #1 loop each corner and dot product the distance
//	//		then the furthest negative distance is the corner of greatest overlap
//	// float position_dot_normal =
//	//	a_aabb.getMax().dot(a_line.get_normal());
//	// float distance = position_dot_normal - a_line.get_distance();
//
//	glm::vec2 extents = 0.5f * a_aabb.get_extents();
//	glm::vec2 extents_neg_x = 0.5f * glm::vec2(-a_aabb.get_extents().x, a_aabb.get_extents().y);
//	
//	float dot_extents = extents.dot(a_line.get_normal());
//	float dot_extents_neg_x = extents_neg_x.dot(a_line.get_normal());
//
//	float radius = fmaxf(fabsf(dot_extents), fabsf(dot_extents_neg_x));
//
//	circle projection{ a_aabb.getPosition() , radius };
//	return circle_vs_line(projection, a_line);
//}
//
//glm::vec2 collision_manager::clamp(
//	const glm::vec2& a_vector2,
//	const glm::vec2& a_min,
//	const glm::vec2& a_max)
//{
//	return { fminf(fmaxf(a_glm::vec2.x, a_min.x), a_max.x),
//			 fminf(fmaxf(a_glm::vec2.y, a_min.y), a_max.y) };
//}