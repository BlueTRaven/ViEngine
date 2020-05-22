#version 330 core

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 object;
uniform mat4 object_normal;
uniform vec3 tint_color;

uniform mat4 depth_bias;
uniform mat4 depth_projection;
uniform mat4 depth_camera;
uniform mat4 depth_object;

in vec2 tex_coord;
in vec4 color;
in vec3 position;
in vec3 normal;

out vec2 frag_tex_coord;
out vec4 frag_color;
out vec3 frag_pos_ls;	//local space
out vec3 frag_pos_ws;	//world space
out vec3 frag_pos_ss;	//screen (view) space
out vec3 frag_pos_cs;	//clip space
out vec3 frag_normal;

out vec3 shadow_coord;	//position of the vertex as seen by the depth (light)

void main()
{
	gl_Position = projection * camera * object * vec4(position, 1.0);
	
	shadow_coord = (depth_bias * depth_projection * depth_camera * depth_object * vec4(position, 1.0)).xyz;

	frag_tex_coord = tex_coord;
	frag_color = color * vec4(tint_color, 1);
	frag_pos_ls = position;
	frag_pos_ws = vec3(object * vec4(position, 1.0));
	frag_pos_ss = vec3(camera * vec4(frag_pos_ws, 1.0));
	frag_pos_cs = vec3(projection * vec4(1.0, frag_pos_ss));
	
	frag_normal = normalize(mat3(object_normal) * normal);
}