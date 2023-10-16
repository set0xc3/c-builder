#version 460 core

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec4 a_color;

out vec4 v_color;

uniform mat4 u_proj;

void
main() 
{
	gl_Position = u_proj * vec4(a_pos, 0.0, 1.0f);
	v_color = a_color;
}
