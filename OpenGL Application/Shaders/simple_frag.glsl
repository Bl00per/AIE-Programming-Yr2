//Classic Phong fragment shader
#version 450

in vec4 v_position;
in vec3 v_normal; 

uniform sampler2D diffuse_texture; 

//uniform vec3 Ka;				//Ambient mat colour
//uniform vec3 Kd;				//Diffuse mat colour
//uniform vec3 Ks;				//Specular mat colour
uniform float specular_power = 32;	//Mat specular power
//
//uniform vec3 Ia;				//Ambient light colour
//uniform vec3 Id;				//Diffuse light colour
//uniform vec3 Is;				//Specular light colour 

uniform vec3 light_direction;

uniform vec3 camera_position; 

in vec2 final_texture_coordinates;
out vec4 frag_colour; 

void main()
{
	//Ensure normal and light direction are normalised 
	vec3 N = normalize(v_normal);
	vec3 L = normalize(light_direction);

	//Calculate lambert term (negate light direction)
	float lambert_term = max(0, min(1, dot(N, -L))); 
	//frag_colour = vec4(lambert_term, lambert_term, lambert_term, 1);

	//Calculate view vector and reflection vector
	vec3 V = normalize(camera_position - v_position.xyz);
	vec3 R = reflect(L, N);

	//Calculate specular term
	float specular_term = pow(max(0, dot(R, V)), specular_power); 

	//Calculate each colour property
	//vec3 ambient = Ia * Ka;
	vec3 diffuse = vec3(lambert_term, lambert_term, lambert_term);////Id * Kd * lambert_term;
	vec3 specular = vec3(specular_term, specular_term, specular_term);////Is * Ks * specular_term;

	//vec4 col = texture(diffuse_texture, final_texture_coodinates);
	//if (col.a < 0.0001)
		//discard;
	//frag_colour = vec4(ambient + diffuse + specular, 1);
	//frag_colour = vec4(light_direction, 1);
	//frag_colour = vec4(v_normal, 1);
	//vec4 result = vec4(ambient + diffuse + specular, 1); 
	//frag_colour = (result + col);
	frag_colour = vec4(diffuse + specular, 1);
}
