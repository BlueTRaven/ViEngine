#version 330 core

uniform vec3 light_pos;
uniform vec3 light_color;

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
	vec3 pos_to_light = light_pos - frag_pos;
	float brightness = dot(frag_normal, pos_to_light) / (length(pos_to_light) * length(frag_normal));
	brightness = clamp(brightness, 0, 1);
	
	vec4 tex_color = texture(tex, frag_tex_coord);
	color = vec4(brightness * light_color * tex_color.rgb * frag_color.rgb, tex_color.a * frag_color.a);
}