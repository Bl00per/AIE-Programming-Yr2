#include <iostream>
#include <fstream>
#include <sstream>
#include "glm.hpp"
#include "ext.hpp"
#include "gl_core_4_5.h"
#include "glfw3.h"
#include "free_camera.h"
#include "OBJMesh.h"
#include "Shader.h"
#include "Mesh.h"
#include "Primitives.h"

using uint = unsigned int;


void PrintErrorLog(uint ID);
//void process_input(GLFWwindow* window);
//void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
//void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

// Delta time
double delta_time = 0.0f;
double last_frame = 0.0f;

// Settings
int width, height;
const unsigned int SCR_WIDTH = width;
const unsigned int SCR_HEIGHT = height;

bool pause_camera = false;

// Camera
free_camera main_camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));

int main()
{
#pragma region Initialise
	// Initialise Everything
	if (glfwInit() == false)
		return -1;

	// Setting Window Resolution
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Hello World!", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i.\n", major, minor);

	glEnable(GL_DEPTH_TEST);

	glfwGetWindowSize(window, &width, &height);
#pragma endregion

#pragma region Shader
	Shader* cshader = new Shader(".\\Shaders\\simple_vertex.glsl", ".\\Shaders\\textured_frag_shader.glsl");
	Shader* bshader = new Shader(".\\Shaders\\simple_vertex.glsl", ".\\Shaders\\simple_frag.glsl");
	
	// ---CHAIR---
	cshader->m_light.ambient = { 0.1f, 0.1f, 0.1f }; 	// Dark grey ambient
	// White Light
	cshader->m_light.diffuse = { 1, 1, 1 };
	cshader->m_light.specular = { 1, 1, 1 };

	// ---BUNBUN---
	bshader->m_light.ambient = { 0.1f, 0.1f, 0.1f }; 	// Dark grey ambient
	// White Light
	bshader->m_light.diffuse = { 1, 1, 1 };
	bshader->m_light.specular = { 1, 1, 1 };
#pragma endregion

#pragma region Model
	aie::OBJMesh chair_mesh;
	aie::OBJMesh bunbun_mesh;
	//Override the materials here because they aren't getting loaded from an mtl file.
	//obj_mesh.m_object_material[0].ambient = glm::vec3(1, 1, 1);
	//obj_mesh.m_object_material[0].diffuse = glm::vec3(1, 1, 1);
	//obj_mesh.m_object_material[0].specular = glm::vec3(1, 1, 1);
	//Mesh* cube = Primitives::cube();
	//Mesh* plane = Primitives::plane();
	chair_mesh.load("./Models/Chair/Regency_low_optimized.obj", false);
	bunbun_mesh.load("./Models/Bunny.obj");


	glm::mat4 chair_model = glm::mat4(0.01);
	chair_model[3][3] = 1.0f; // Resetting model position cause it's tiny
	glm::mat4 bunbun_model = glm::mat4(1);
#pragma endregion

#pragma region Texture
	//Texture* texture_1 = new Texture(".\\Textures\\unknown.png");

	// Diffuse Texture
	Texture diffuse("./Models/Chair/Regency_low_my_Divani_Chester_nuovi_regency_mat_Diffuse.png");
	// Initialize our shader
	cshader->use();
	// Initialize our diffuse texture
	int diffuse_location = glGetUniformLocation(cshader->shader_ID, "diffuse_texture");
	glUniform1i(diffuse_location, 0);

	// Normals texture
	Texture normals("./Models/Chair/Regency_low_my_Divani_Chester_nuovi_regency_mat_Normal.png");
	// Initialize our normals texture
	int normals_location = glGetUniformLocation(cshader->shader_ID, "normal_texture");
	glUniform1i(normals_location, 1);
#pragma endregion

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Background Colour
	// Game Loop
	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = glfwGetTime();

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			pause_camera = !pause_camera;

		if (pause_camera)
		{
			glm::vec3 paused_direction = glm::normalize(glm::vec3(1, 1, 1));
		}
		else
			cshader->m_light.direction = glm::normalize(glm::vec3(glm::cos((time) * 2), glm::sin((time) * 2), 0));

		double current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		// ---Camera---
		main_camera.update(delta_time);

		// ---Texturing---
		cshader->use();
		// Render the diffuse texture
		glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
		glBindTexture(GL_TEXTURE_2D, diffuse.texture);
		// Render the normal texture
		glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
		glBindTexture(GL_TEXTURE_2D, normals.texture);

		// ---CHAIR---
		// PVM
		cshader->setMat4("projection_view_matrix", main_camera.get_projection_view_transform());
		cshader->setMat4("model_matrix", chair_model);
		glm::mat3 chair_normal_matrix = { glm::normalize(glm::vec3(chair_model[0])),
									glm::normalize(glm::vec3(chair_model[1])),
									glm::normalize(glm::vec3(chair_model[2])) };
		cshader->setMat3("normal_matrix", glm::inverseTranspose(chair_normal_matrix));
		cshader->setVec3("camera_position", main_camera.get_position());
		cshader->setVec3("light_direction", cshader->m_light.direction);

		// Material Colour
		cshader->setVec3("Ka", glm::vec3(1, 1, 1));
		cshader->setVec3("Kd", glm::vec3(1, 1, 1));
		cshader->setVec3("Ks", glm::vec3(1, 1, 1));
		cshader->setFloat("specular_power", 32.0f);

		// Light Colour
		cshader->setVec3("Ia", cshader->m_light.ambient);
		cshader->setVec3("Id", cshader->m_light.diffuse);
		cshader->setVec3("Is", cshader->m_light.specular);



		// ---BUNBUN---
		// PVM
		bshader->setMat4("projection_view_matrix", main_camera.get_projection_view_transform());
		bshader->setMat4("model_matrix", bunbun_model);
		glm::mat3 bun_normal_matrix = { glm::normalize(glm::vec3(bunbun_model[0])),
									glm::normalize(glm::vec3(bunbun_model[1])),
									glm::normalize(glm::vec3(bunbun_model[2])) };
		bshader->setMat3("normal_matrix", glm::inverseTranspose(bun_normal_matrix));
		bshader->setVec3("camera_position", main_camera.get_position());
		bshader->setVec3("light_direction", bshader->m_light.direction);
		cshader->setFloat("specular_power", 32.0f);


		chair_mesh.draw();
		bunbun_mesh.draw();
		//plane->draw(pshader/*, texture_1*/); 

#pragma region Wireframe Mode
		//static unsigned char wireframe;

		//if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		//	glPolygonMode(GL_FRONT_AND_BACK, (wireframe = 1 - wireframe) ? GL_LINE : GL_FILL);

		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
#pragma endregion

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void PrintErrorLog(uint ID)
{
	// Get the length of OpenGL error message
	GLint log_length = 0;
	glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &log_length);
	// Create the error buffer
	char* log = new char[log_length];
	// Copy the error from the buffer
	glGetShaderInfoLog(ID, log_length, 0, log);

	// Create the error message
	std::string error_message(log);
	error_message += "SHADER_FAILED_TO_COMPILE";
	std::cout << error_message.c_str();
	// Clean up anyway
	delete[] log;
}