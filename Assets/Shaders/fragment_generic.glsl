#version 330 core

out vec4 color;

in vec4 frag_color;
in vec2 frag_tex_coord;	//tex coords need to be inverted for whatever reason.

uniform sampler2D tex;

void main()
{
	//color = vec4(vec3(gl_FragCoord.z), 1.0);
	color = texture(tex, frag_tex_coord) * frag_color; 
}
