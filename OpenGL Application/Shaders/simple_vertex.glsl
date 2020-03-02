#version 450

layout(location = 0) in vec3 local_position;
layout(location = 1) in vec2 texture_coordinates;
layout(location = 2) in vec4 normal;

out vec4 v_position;
out vec3 v_normal;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix; 

uniform mat3 normal_matrix;

out vec2 final_texture_coordinates;


void main()
{
	final_texture_coordinates = texture_coordinates;
	v_position = model_matrix * vec4(local_position, 1);
	v_normal = normal_matrix * normal.xyz;
	gl_Position = (projection_view_matrix * model_matrix) * vec4(local_position, 1);
}