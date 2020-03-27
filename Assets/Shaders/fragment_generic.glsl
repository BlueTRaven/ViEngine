#version 330 core

out vec4 color;

in vec4 frag_color;
in vec2 frag_tex_coord;	//tex coords need to be inverted for whatever reason.
in vec3 frag_normal;
in vec3 frag_pos;

in vec3 frag_diffuse_pos;

uniform vec3 diffuse_color;
//uniform vec3 diffuse_pos;

uniform vec3 ambient_color;
uniform float ambient_strength;

uniform sampler2D tex;

void main()
{
	vec3 ambient = ambient_strength * ambient_color;
	
	vec3 norm = normalize(frag_normal);
	vec3 light_dir = normalize(frag_diffuse_pos - frag_pos);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff * diffuse_color;
	
	float spec_strength = 0.5;
	vec3 view_dir = normalize(-frag_pos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
	vec3 specular = diffuse_color * spec * spec_strength;
	
	vec4 tex_color = texture(tex, frag_tex_coord) * frag_color;
	vec3 result = ambient + diffuse + specular;
	color =  vec4(result, 1.0) * tex_color;
}
