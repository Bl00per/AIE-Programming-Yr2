#pragma once

#include <string>
#include <vector>
#include "glm.hpp"
#include "Shader.h"
#include "Texture.h"

using uint = unsigned int;

struct Vertex
{
#pragma region Constructors
	Vertex(glm::vec3 new_position)
	{
		position = new_position;
		texture_coords = glm::vec2(0);
		normal = glm::vec4(0);
	}
	Vertex(float x, float y, float z)
	{
		position.x = x;
		position.y = y;
		position.z = z;
		texture_coords = glm::vec2(0);
		normal = glm::vec4(0);
	}
	Vertex(float x, float y, float z, float u, float v)
	{
		position.x = x;
		position.y = y;
		position.z = z;
		texture_coords.x = u;
		texture_coords.y = v;
		normal = glm::vec4(0);
	}
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz, float nw)
	{
		position.x = x;
		position.y = y;
		position.z = z;
		texture_coords.x = u;
		texture_coords.y = v;
		normal.x = nx;
		normal.y = ny;
		normal.z = nz;
		normal.w = nw;
	}
#pragma endregion

	glm::vec3 position;
	glm::vec2 texture_coords;
	glm::vec4 normal;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> a_vertices, std::vector<uint> a_index);
	~Mesh();

	// Vertex positions
	std::vector<Vertex> vertices;
	// Vertex connection order
	std::vector<uint> index;

	// Render data
	uint VAO, VBO, IBO;

	// Draw shapes with the given shader
	void draw(Shader* shader);
	void draw(Shader*, Texture* texture);

	void set_up_mesh();
};