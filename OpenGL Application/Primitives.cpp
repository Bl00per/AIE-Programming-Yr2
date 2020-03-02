#include "Primitives.h"

// For when I want a circle or sphere later
#define PI 3.1415926535897932384626433832795f

Mesh* Primitives::cube()
{
	std::vector<Vertex> cube_verticies
	{
		//Back plane 
		/*0*/Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0, 0, -1, 0),	//back bottom left
		/*1*/Vertex(-0.5f, 0.5f, -0.5f,  1.0f, 0.0f, 0, 0, -1, 0),	//back top left

		/*2*/Vertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0, 0, -1, 0),		//back top right
		/*3*/Vertex(0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0, 0, -1, 0),	//back bottom right
		//---------------------------------------------------------------------
		
		////Front plane 
		/*4*/Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0, 0, 1, 0),	//front bottom left
		/*5*/Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0, 0, 1, 0),		//front top left
		
		/*6*/Vertex(0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0, 1, 0, 0),		//front top right
		/*7*/Vertex(0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0, 1, 0, 0),		//front bottom right
		//---------------------------------------------------------------------
		
		//Left plane
		/*8*/Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0, 1, 0, 0),	//front bottom left
		/*9*/Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0, 1, 0, 0),	//back bottom left
		
		/*10*/Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0, 1, 0, 0),	//front top left
		/*11*/Vertex(-0.5f, 0.5, 0.5f, 1.0f, 0.0f, 0, 1, 0, 0),		//back top left
		//---------------------------------------------------------------------
		
		//Right plane
		/*12*/Vertex(0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0, 1, 0, 0),	//front bottom right
		/*13*/Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0, 1, 0, 0),	//back bottom right
		
		/*14*/Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0, 1, 0, 0),	//front top right
		/*15*/Vertex(0.5f, 0.5, 0.5f, 0.0f, 0.0f, 0, 1, 0, 0),		//back top right
		//---------------------------------------------------------------------
		
		//Top plane 
		/*16*/Vertex(-0.5f, 0.5f, -0.5f,  0.0f, 0.0f, 0, 1, 0, 0),	//back top left
		/*17*/Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0, 1, 0, 0),	//front top left
		
		/*18*/Vertex(0.5f, 0.5f, 0.5f,  1.0f, 1.0f, 0, 1, 0, 0),	//front top right
		/*19*/Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0, 1, 0, 0),	//back top right
		//---------------------------------------------------------------------
		
		//Bottom plane
		/*20*/Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0, 1, 0, 0),	//back bottom left
		/*21*/Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0, 1, 0, 0),	//front bottom left
		
		/*22*/Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0, 1, 0, 0),	//front bottom right
		/*23*/Vertex(0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0, 1, 0, 0),	//back bottom right
	};

	std::vector<uint> cube_indices
	{
		//Front---------------------------
		0, 1, 3,
		2, 3, 1,

		//Back----------------------------
		7, 5, 4,
		5, 7, 6,

		//Left----------------------------
		8, 9, 11,
		10, 8, 11,

		//Right---------------------------
		13, 12, 15,
		14, 15, 12,

		//Top-----------------------------
		16, 17, 19,
		18, 19, 17,

		//Bottom--------------------------
		20, 23, 21,
		22, 21, 23
	};

	return new Mesh(cube_verticies, cube_indices);
}

Mesh* Primitives::plane()
{
	std::vector<Vertex> plane_vertices
	{
		Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0, 0, 1, 0),	//front bottom left
		Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0, 0, 1, 0),		//front top left
		Vertex(0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0, 1, 0, 0),		//front top right
		Vertex(0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0, 1, 0, 0)		//front bottom right
	};

	std::vector<uint> plane_indices
	{
		0, 1, 3,
		1, 2, 3
	};

	return new Mesh(plane_vertices, plane_indices);
}