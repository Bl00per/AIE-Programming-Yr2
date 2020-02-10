#include <iostream>
#include <fstream>
#include <sstream>
#include "glm.hpp"
#include "ext.hpp"
#include "gl_core_4_5.h"
#include "glfw3.h"


using uint = unsigned int;

void PrintErrorLog(uint ID);
void ProcessInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double x_pos, double y_pos);
void ScrollCallback(GLFWwindow* window, double x_offset, double y_offset);

// Camera Variables
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

// Delta time
float delta_time = 0.0f;
float last_frame = 0.0f;

// Mouse values
bool first_mouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float last_x = 640.0f, last_y = 480.0f;
float fov = 45.0f;

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
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

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
#pragma endregion

	// Mesh Data
	glm::vec3 vertices[] =
	{
	glm::vec3(-0.5f, -0.5f, -0.5f),
	glm::vec3(0.5f, -0.5f, -0.5f),
	glm::vec3(0.5f,  0.5f, -0.5f),
	glm::vec3(0.5f,  0.5f, -0.5f),
	glm::vec3(-0.5f,  0.5f, -0.5f),
	glm::vec3(-0.5f, -0.5f, -0.5f),

	glm::vec3(-0.5f, -0.5f, 0.5f),
	glm::vec3(0.5f, -0.5f, 0.5f),
	glm::vec3(0.5f,  0.5f, 0.5f),
	glm::vec3(0.5f,  0.5f, 0.5f),
	glm::vec3(-0.5f,  0.5f, 0.5f),
	glm::vec3(-0.5f, -0.5f, 0.5f),

	glm::vec3(-0.5f,  0.5f, 0.5f),
	glm::vec3(-0.5f,  0.5f, -0.5f),
	glm::vec3(-0.5f, -0.5f, -0.5f),
	glm::vec3(-0.5f, -0.5f, -0.5f),
	glm::vec3(-0.5f, -0.5f, 0.5f),
	glm::vec3(-0.5f,  0.5f, 0.5f),

	glm::vec3(0.5f,  0.5f, 0.5f),
	glm::vec3(0.5f,  0.5f, -0.5f),
	glm::vec3(0.5f, -0.5f, -0.5f),
	glm::vec3(0.5f, -0.5f, -0.5f),
	glm::vec3(0.5f, -0.5f, 0.5f),
	glm::vec3(0.5f,  0.5f, 0.5f),

	glm::vec3(-0.5f, -0.5f, -0.5f),
	glm::vec3(0.5f, -0.5f, -0.5f),
	glm::vec3(0.5f, -0.5f, 0.5f),
	glm::vec3(0.5f, -0.5f, 0.5f),
	glm::vec3(-0.5f, -0.5f, 0.5f),
	glm::vec3(-0.5f, -0.5f, -0.5f),

	glm::vec3(-0.5f,  0.5f, -0.5f),
	glm::vec3(0.5f,  0.5f, -0.5f),
	glm::vec3(0.5f,  0.5f, 0.5f),
	glm::vec3(0.5f,  0.5f, 0.5f),
	glm::vec3(-0.5f,  0.5f, 0.5f),
	glm::vec3(-0.5f,  0.5f, -0.5f)
	};

	int number_of_verts = 36;

	int index_buffer[]{
		0,1,2,
		1,2,3
	};

	float texCoords[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};

	// Create and 'Load' Mesh
	uint VAO;
	uint VBO;
	uint IBO;

	// Shader ID's
	uint vertex_shader_ID = 0;
	uint fragment_shader_ID = 0;
	uint shader_program_ID = 0;

#pragma region Buffer Stuff
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, number_of_verts * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), index_buffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#pragma endregion

#pragma region Camera
	glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 camera_direction = glm::normalize(camera_pos - camera_target);
	glm::vec3 camera_right = glm::normalize(glm::cross(camera_up, camera_direction));

	glm::mat4 view;
	view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	//glm::mat4 projection = glm::perspective(1.507f, 16 / 9.0f, 0.1f, 5.0f);
	//glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0, 1, 0));
#pragma endregion

#pragma region Vertex Shader
	// Load shader from file into string
	std::string shader_data;
	std::ifstream in_file_stream(".\\Shaders\\simple_vertex.glsl", std::ifstream::in);

	// Load the source into a string for compliation
	std::stringstream string_stream;
	if (in_file_stream.is_open())
	{
		string_stream << in_file_stream.rdbuf();
		shader_data = string_stream.str();
		in_file_stream.close();
	}

	// Allocate space for shader program
	vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);
	// Convert to raw char*
	const char* data = shader_data.c_str();
	// Send in the char* to shader location
	glShaderSource(vertex_shader_ID, 1, (const GLchar**)&data, 0);
	// Build!
	glCompileShader(vertex_shader_ID);

	// Did it work?
	GLint success = GL_FALSE;
	glGetShaderiv(vertex_shader_ID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		PrintErrorLog(vertex_shader_ID);
	}
#pragma endregion

#pragma region Fragment Shader
	// Load shader from file into string
	std::ifstream in_file_stream_frag(".\\Shaders\\simple_frag.glsl", std::ifstream::in);
	std::stringstream frag_string_stream;

	// Load the source into a string for compliation
	if (in_file_stream_frag.is_open())
	{
		frag_string_stream << in_file_stream_frag.rdbuf();
		shader_data = frag_string_stream.str();
		in_file_stream_frag.close();
	}

	// Allocate space for shader program
	fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);
	// Convert to raw char*
	data = shader_data.c_str();
	// Send in the char* to shader location
	glShaderSource(fragment_shader_ID, 1, (const GLchar**)&data, 0);
	// Build!
	glCompileShader(fragment_shader_ID);

	// Did it work?
	success = GL_FALSE;
	glGetShaderiv(fragment_shader_ID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		PrintErrorLog(fragment_shader_ID);
	}
#pragma endregion

#pragma region Shader Linking
	// Finally link them
	shader_program_ID = glCreateProgram();

	// Attach both shaders by ID and type
	glAttachShader(shader_program_ID, vertex_shader_ID);
	glAttachShader(shader_program_ID, fragment_shader_ID);

	// Link the two programs
	glLinkProgram(shader_program_ID);

	success = GL_FALSE;
	glGetProgramiv(shader_program_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		// Get the length of OpenGL error message
		GLint log_length = 0;
		glGetShaderiv(shader_program_ID, GL_INFO_LOG_LENGTH, &log_length);
		// Create the error buffer
		char* log = new char[log_length];
		// Copy the error from the buffer
		glGetProgramInfoLog(shader_program_ID, log_length, 0, log);

		// Create the error message
		std::string error_message(log);
		error_message += "SHADER_FAILED_TO_COMPILE";
		std::cout << error_message.c_str();
		// Clean up anyway
		delete[] log;
	}
#pragma endregion

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Background Colour

	// Game Loop
	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		ProcessInput(window);

		// ---Model---
		glm::mat4 model = glm::mat4(1);
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		// ---View---
		glm::mat4 view;
		view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

		// ---Projection---
		glm::mat4 projection = glm::mat4(1);
		projection = glm::perspective(glm::radians(fov), 1280.0f / 960.0f, 0.1f, 100.0f);

		glm::mat4 pv = projection * view;
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Set colour of cube

		glUseProgram(shader_program_ID);
		auto uniform_location = glGetUniformLocation(shader_program_ID, "projection_view_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(pv));
		uniform_location = glGetUniformLocation(shader_program_ID, "model_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(model));
		uniform_location = glGetUniformLocation(shader_program_ID, "color");
		glUniform4fv(uniform_location, 1, glm::value_ptr(color));


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

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, number_of_verts);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);

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

void ProcessInput(GLFWwindow* window)
{
	const float camera_speed = 2.5f * delta_time;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_pos += camera_speed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_pos -= camera_speed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera_pos += camera_speed * camera_up;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera_pos -= camera_speed * camera_up;
}

void MouseCallback(GLFWwindow* window, double x_pos, double y_pos)
{
	if (first_mouse)
	{
		last_x = x_pos;
		last_y = y_pos;
		first_mouse = false;
	}

	float x_offset = x_pos - last_x;
	float y_offset = last_y - y_pos;
	last_x = x_pos;
	last_y = y_pos;

	const float sensitivity = 0.1f;
	x_offset *= sensitivity;
	y_offset *= sensitivity;

	yaw += x_offset;
	pitch += y_offset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera_front = glm::normalize(direction);
}

void ScrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
	if (fov > 1.0f && fov < 45.0f)
		fov -= y_offset;
	else if (fov <= 1.0f)
		fov = 1.0f;
	else if (fov >= 45.0f)
		fov = 45.0f;
}