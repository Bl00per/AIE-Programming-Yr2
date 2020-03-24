#pragma once
#include "circle.h"
#include "line.h"
#include "alignedBoundingBox.h"

class collision_manager
{
public:
	static bool circle_vs_circle(const circle& a_circle_A,
								 const circle& a_circle_B);

	//static bool aabb_vs_aabb(const alignedBoundingBox& a_aabb_A,
	//						 const alignedBoundingBox& a_aabb_B);

	//static bool aabb_vs_circle(const alignedBoundingBox& a_aabb,
	//						   const circle& a_circle);
//
//	static bool circle_vs_line(const circle& a_circle,
//							   const line& a_line);
//
//	static bool aabb_vs_line(const alignedBoundingBox& a_aabb,
//							 const line& a_line);
//
//private:
//	static glm::vec2 clamp(const glm::vec2& a_vector2,
//						  const glm::vec2& a_min,
//						  const glm::vec2& a_max);
};

