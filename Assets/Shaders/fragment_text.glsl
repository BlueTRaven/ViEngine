#version 330 core

out vec4 color;

in vec4 frag_color;
in vec2 frag_tex_coord;	//tex coords need to be inverted for whatever reason.

uniform sampler2D tex;

void main()
{
	float mon_col = texture(tex, frag_tex_coord).r;
	vec4 fcol = vec4(mon_col, mon_col, mon_col, mon_col);
	color = fcol * frag_color; 
}
