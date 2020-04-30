#version 330 core

layout(location = 0) out vec4 color;

in vec4 frag_color;
in vec2 frag_tex_coord;
//world space normal
in vec3 frag_normal;
//world space position
in vec3 frag_pos;

uniform sampler2D tex;

void main()
{
	vec4 fcolor = texture(tex, frag_tex_coord);
	color = vec4(vec3(1.0, 1.0, 1.0) - fcolor.rgb, fcolor.a) * frag_color;
}