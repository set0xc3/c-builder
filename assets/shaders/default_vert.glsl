#version 460 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec4 a_color;

out vec4 v_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void
main() 
{
	gl_Position = u_projection * u_model * vec4(a_position.xy, 0.0, 1.0f);
	v_color = a_color;
}
