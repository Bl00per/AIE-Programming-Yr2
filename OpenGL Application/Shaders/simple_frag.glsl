//Classic Phong fragment shader
#version 450

in vec4 v_position;
in vec3 v_normal; 

uniform sampler2D diffuse_texture; 

uniform float specular_power;

uniform vec4 color;
uniform vec3 camera_position; 
uniform vec3 light_direction;

in vec2 final_texture_coordinates;
out vec4 frag_colour; 

void main()
{
	//Ensure normal and light direction are normalised 
	vec3 N = normalize(v_normal);
	vec3 L = normalize(light_direction);

	//Calculate lambert term (negate light direction)
	float lambert_term = max(0, min(1, dot(N, -L))); 

	//Calculate view vector and reflection vector
	vec3 V = normalize(camera_position - v_position.xyz);
	vec3 R = reflect(L, N);

	//Calculate specular term
	float specular_term = pow(max(0, dot(R, V)), specular_power); 

	//Calculate each colour property
	vec3 diffuse = vec3(lambert_term, lambert_term, lambert_term);
	vec3 specular = vec3(specular_term, specular_term, specular_term);

	//frag_colour = vec4(diffuse + specular, 1) + color;
	frag_colour = vec4(diffuse + specular, 1);
}
