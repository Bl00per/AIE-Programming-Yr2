#include <iostream>
#include "glm.hpp"
#include "ext.hpp"
#include "gl_core_4_5.h"
#include "glfw3.h"

#include <fstream>
#include <sstream>

using uint = unsigned int;

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
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i.\n", major, minor);
#pragma endregion

	// Mesh Data
	glm::vec3 verticies[] =
	{
		glm::vec3(-0.5f,  0.5f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f,   0.5f, 0.0f),
		glm::vec3(0.5f,  -0.5f, 0.0f)
	};

	//int number_of_verts = 10;
	int index_buffer[]{ 
		0,1,2,
		1,2,3
};

	// Create and 'Load' Mesh
	uint VAO;
	uint VBO;
	uint IBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), verticies, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), index_buffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Camera
	glm::mat4 projection = glm::perspective(1.507f, 16 / 9.0f, 0.4f, 5.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 model = glm::mat4(1);

	uint vertex_shader_ID = 0;
	uint fragment_shader_ID = 0;
	uint shader_program_ID = 0;

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
		// Get the length of OpenGL error message
		GLint log_length = 0;
		glGetShaderiv(vertex_shader_ID, GL_INFO_LOG_LENGTH, &log_length);
		// Create the error buffer
		char* log = new char[log_length];
		// Copy the error from the buffer
		glGetShaderInfoLog(vertex_shader_ID, log_length, 0, log);

		// Create the error message
		std::string error_message(log);
		error_message += "VERTEX_SHADER_FAILED_TO_COMPILE";
		std::cout << error_message.c_str();
		// Clean up anyway
		delete[] log;
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
		// Get the length of OpenGL error message
		GLint log_length = 0;
		glGetShaderiv(fragment_shader_ID, GL_INFO_LOG_LENGTH, &log_length);
		// Create the error buffer
		char* log = new char[log_length];
		// Copy the error from the buffer
		glGetShaderInfoLog(fragment_shader_ID, log_length, 0, log);

		// Create the error message
		std::string error_message(log);
		error_message += "FRAGMENT_SHADER_FAILED_TO_COMPILE";
		std::cout << error_message.c_str();
		// Clean up anyway
		delete[] log;
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
	glGetShaderiv(shader_program_ID, GL_LINK_STATUS, &success);
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

	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Game Loop
	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static int framecount = 0;
		framecount++;

		model = glm::rotate(model, 0.016f, glm::vec3(0, 1, 0));

		glm::mat4 pv = projection * view;
		glm::vec4 color = glm::vec4(0.0f, 0.8f, 1.0f, 1.0f);

		glUseProgram(shader_program_ID);
		auto uniform_location = glGetUniformLocation(shader_program_ID, "projection_view_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(pv));	
		uniform_location = glGetUniformLocation(shader_program_ID, "model_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(model));
		uniform_location = glGetUniformLocation(shader_program_ID, "color");
		glUniform4fv(uniform_location, 1, glm::value_ptr(color));

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, number_of_verts);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}