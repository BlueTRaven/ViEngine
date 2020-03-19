#version 330 core

uniform mat4 projection;
uniform mat4 object;

uniform vec4 tint_color;

in vec2 tex_coord;
in vec4 color;
in vec3 position;

out vec2 frag_tex_coord;
out vec4 frag_color;

void main()
{
	gl_Position = projection * object * vec4(position, 1.0);

	frag_tex_coord = tex_coord;
	frag_color = color * tint_color;
}

