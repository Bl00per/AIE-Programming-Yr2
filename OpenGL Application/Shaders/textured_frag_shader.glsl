//Textured Phong fragment shader
#version 450

in vec4 v_position;
in vec3 v_normal; 
in vec3 v_tangent;

uniform sampler2D diffuse_texture;
uniform sampler2D normal_texture; 

uniform vec3 Ka;	//Ambient mat colour
uniform vec3 Kd;	//Diffuse mat colour
uniform vec3 Ks;	//Specular mat colour
//uniform float specular_power = 32;
uniform float specular_power;	//Mat specular power

uniform vec3 Ia;	//Ambient light colour
uniform vec3 Id;	//Diffuse light colour
uniform vec3 Is;	//Specular light colour 

uniform vec3 camera_position; 
uniform vec3 light_direction;

in vec2 final_texture_coordinates;
out vec4 frag_colour; 

void main()
{
	//Ensure normal and light direction are normalised 
	vec3 N = normalize(v_normal);
	vec3 T = normalize(v_tangent);
	vec3 B = cross(v_normal, v_tangent);

	mat3 TBN = mat3(T, B, N);
	vec3 normals = texture2D(normal_texture, final_texture_coordinates).xyz * 2 - 1;
	N = TBN * normals;

	vec3 L = normalize(light_direction);

	//Calculate lambert term (negate light direction)
	float lambert_term = max(0, min(1, dot(N, -L)));

	//Calculate view vector and reflection vector
	vec3 V = normalize(camera_position - v_position.xyz);
	vec3 R = reflect(L, N);

	//Calculate specular term
	float specular_term = pow(max(0, dot(R, V)), specular_power); 

	vec3 diffuse_texture = texture2D(diffuse_texture, final_texture_coordinates).xyz;
	
	//Calculate each colour property
	vec3 ambient = Ia * Ka;
	//vec3 diffuse = texture2D(diffuse_texture, final_texture_coordinates).xyz * lambert_term;
	//vec3 specular = vec3(specular_term, specular_term, specular_term);
	vec3 diffuse = Id * Kd * diffuse_texture * lambert_term;
	vec3 specular = Is * Ks * vec3(specular_term, specular_term, specular_term);

	//vec4 col = texture(diffuse_texture, final_texture_coodinates);
	//if (col.a < 0.0001)
	//	discard;
	frag_colour = vec4(ambient + diffuse + specular, 1);
	//frag_colour = vec4(1, 0, 0, 1);
	//frag_colour = vec4(Ka, 1);
}
