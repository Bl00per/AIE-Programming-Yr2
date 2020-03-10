#version 450

layout(location = 0) in vec4 local_position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texture_coordinates;
layout(location = 3) in vec4 tangent;

out vec4 v_position;
out vec3 v_normal;
out vec3 v_tangent;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix; 

uniform mat3 normal_matrix;

out vec2 final_texture_coordinates;


void main()
{
	final_texture_coordinates = texture_coordinates;
	v_position = model_matrix * local_position;
	v_normal = normal_matrix * normal.xyz;
	v_tangent = normal_matrix * tangent.xyz;
	gl_Position = (projection_view_matrix * model_matrix) * local_position;
}