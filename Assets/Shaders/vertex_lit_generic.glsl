#version 330 core

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 object;

in vec2 tex_coord;
in vec4 color;
in vec3 position;
in vec3 normal;

out vec2 frag_tex_coord;
out vec4 frag_color;
out vec3 frag_pos;
out vec3 frag_normal;

void main()
{
	gl_Position = projection * camera * object * vec4(position, 1.0);

	frag_tex_coord = tex_coord;
	frag_color = color;
	frag_pos = vec3(object * vec4(position, 1.0));
	frag_normal = normalize(transpose(inverse(mat3(object))) * normal);
}