#version 330 core

//uniform vec3 light_pos;
//uniform vec3 light_color;

struct DiffuseLight 
{
	vec3 position;
	vec3 color;
	float strength;
};

struct RadialFog
{
	vec3 color;
	float start;	//distance in world space at which fog starts
	float end;		//distance in world space at which fog ends
};

uniform DiffuseLight diffuse_light = DiffuseLight(vec3(0, 0, 0), vec3(1, 1, 1), 1);
uniform RadialFog radial_fog = RadialFog(vec3(0, 0, 0), 32, 128);

out vec4 color;

in vec4 frag_color;
in vec2 frag_tex_coord;
//world space normal
in vec3 frag_normal;
//world space position
in vec3 frag_pos;

uniform sampler2D tex;

void main()
{
	//get vector to light from world space position
	vec3 pos_to_light = diffuse_light.position - frag_pos;
	float brightness = dot(frag_normal, pos_to_light) / (length(pos_to_light) * length(frag_normal));
	brightness = clamp(brightness, 0, 1);
	
	vec4 tex_color = texture(tex, frag_tex_coord) * frag_color;
	vec3 diffuse_light_color = brightness * diffuse_light.color * diffuse_light.strength;
	
	color = vec4(tex_color.rgb * diffuse_light_color, tex_color.a);
}