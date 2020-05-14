#version 330 core

layout(location = 0) out float color;

//in vec4 frag_color;
in vec2 frag_tex_coord;
//world space normal
//in vec3 frag_normal;
//world space position
in vec3 frag_pos;
in vec3 frag_pos_ss;

uniform sampler2D tex;

void main()
{
	color = gl_FragCoord.z;
}