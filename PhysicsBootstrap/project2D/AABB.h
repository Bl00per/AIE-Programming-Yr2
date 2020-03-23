#include "RigidBody.h"

#pragma once
#ifndef _AABB_H_
#define _AABB_H_

class AABB : public RigidBody
{
	glm::vec2 extents;
};

#endif // !_AABB_H_