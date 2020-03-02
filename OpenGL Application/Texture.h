#pragma once
#include <string>
#include "glfw3.h"

class Texture
{
public:
	Texture(std::string a_texture_name);

	unsigned int texture;
};