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
float delta_time = 0.0f;
float last_frame = 0.0f;

// Settings
int width, height;

const unsigned int SCR_WIDTH = width;
const unsigned int SCR_HEIGHT = height;

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

#pragma region Old Camera
	//glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::vec3 camera_direction = glm::normalize(camera_pos - camera_target);
	//glm::vec3 camera_right = glm::normalize(glm::cross(camera_up, camera_direction));

	//glm::mat4 view;
	//view = glm::lookAt(
	//	glm::vec3(0.0f, 0.0f, 3.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 1.0f, 0.0f));

	//glm::mat4 projection = glm::perspective(1.507f, 16 / 9.0f, 0.1f, 5.0f);
	//glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0, 1, 0));
#pragma endregion

#pragma region Shader
	Shader* pshader = new Shader(".\\Shaders\\simple_vertex.glsl", ".\\Shaders\\textured_frag_shader.glsl");
#pragma endregion


	// Model
	aie::OBJMesh obj_mesh;
	//Mesh* cube = Primitives::cube();
	Mesh* plane = Primitives::plane();
	obj_mesh.load("./Models/Chair/Regency_low_optimized.obj", false);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Background Colour

	// ---Model---
	// glm::mat4 model = glm::mat4(1);
	glm::mat4 model = glm::mat4(0.01);
	model[3][3] = 1.0f; // Resetting model position cause it's tiny

#pragma region Texture
	Texture* texture_1 = new Texture(".\\Textures\\unknown.png");

	// Diffuse Texture
	Texture diffuse("./Models/Chair/Regency_low_my_Divani_Chester_nuovi_regency_mat_Diffuse.png");
	// Initialize our shader
	pshader->use();
	// Initialize our diffuse texture
	int diffuse_location = glGetUniformLocation(pshader->shader_ID, "diffuse_texture");
	glUniform1i(diffuse_location, 0);

	Texture normals("./Models/Chair/Regency_low_my_Divani_Chester_nuovi_regency_mat_Normal.png");
	int normals_location = glGetUniformLocation(pshader->shader_ID, "normal_texture");
	glUniform1i(normals_location, 1);
#pragma endregion


	// Game Loop
	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		float time = glfwGetTime();
		pshader->m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

		double current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		// ---Camera---
		main_camera.update(delta_time);

		// ---Colour---
		glm::vec4 color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f); // Set colour of cube
		pshader->use();
		// Render the diffuse texture
		glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
		glBindTexture(GL_TEXTURE_2D, diffuse.texture);
		// Render the normal texture
		glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
		glBindTexture(GL_TEXTURE_2D, normals.texture);

		pshader->setMat4("projection_view_matrix", main_camera.get_projection_view_transform());
		pshader->setMat4("model_matrix", model);
		glm::mat3 normal_matrix = { glm::normalize(glm::vec3(model[0])),
									glm::normalize(glm::vec3(model[1])),
									glm::normalize(glm::vec3(model[2])) };
		pshader->setMat3("normal_matrix", glm::inverseTranspose(normal_matrix));
		pshader->setVec4("color", color);
		pshader->setVec3("camera_position", main_camera.get_position());
		pshader->setVec3("light_direction", pshader->m_light.direction);

		obj_mesh.draw();
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

//void process_input(GLFWwindow* window)
//{
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//		main_camera.process_keyboard(camera_movement::FORWARD, delta_time);
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//		main_camera.process_keyboard(camera_movement::BACKWARD, delta_time);
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//		main_camera.process_keyboard(camera_movement::LEFT, delta_time);
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//		main_camera.process_keyboard(camera_movement::RIGHT, delta_time);
//	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
//		main_camera.process_keyboard(camera_movement::UP, delta_time);
//	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
//		main_camera.process_keyboard(camera_movement::DOWN, delta_time);
//}
//
//void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
//{
//	if (first_mouse)
//	{
//		last_x = x_pos;
//		last_y = y_pos;
//		first_mouse = false;
//	}
//
//	float x_offset = x_pos - last_x;
//	float y_offset = last_y - y_pos; // reversed since y-coordinates go from bottom to top
//
//	last_x = x_pos;
//	last_y = y_pos;
//
//	main_camera.process_mouse_movement(x_offset, y_offset);
//}

//void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
//{
//	main_camera.process_mouse_scroll(y_offset);
//}