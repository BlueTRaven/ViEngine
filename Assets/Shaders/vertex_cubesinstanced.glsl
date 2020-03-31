#version 330 core

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 object;
uniform mat4 normal_object;

uniform vec4 tint_color;

uniform vec3 diffuse_pos;

uniform vec3 world_cubes_size;

in vec2 tex_coord;
in vec4 color;
in vec3 position;
in vec3 normal;

in vec3 instance_pos;

out vec2 frag_tex_coord;
out vec4 frag_color;
out vec3 frag_pos;
out vec3 frag_normal;

out vec3 frag_diffuse_pos;

void main()
{
	gl_Position = projection * camera * object * vec4(position + (instance_pos * world_cubes_size), 1.0);

	frag_tex_coord = tex_coord;
	frag_color = color * tint_color;
	frag_pos = vec3(object * vec4(position, 1.0));
	frag_normal = mat3(normal_object) * normal;
	frag_diffuse_pos = vec3(camera * vec4(diffuse_pos, 1.0));
}

