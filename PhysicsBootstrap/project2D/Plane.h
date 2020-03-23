#include "PhysicsObject.h"

#pragma once
#ifndef _PLANE_H_
#define _PLANE_H_

class Plane : public PhysicsObject
{
	glm::vec2 normal: // The plane's 'up' face
	float distance; // From origin
};

#endif // !_PLANE_H_