#version 330 core

layout(location = 0) out float depth;

/*in vec4 frag_color;
in vec2 frag_tex_coord;
//world space normal
in vec3 frag_normal;
//world space position
in vec3 frag_pos;*/

//uniform sampler2D tex;

void main()
{
	depth = gl_FragCoord.z;
}