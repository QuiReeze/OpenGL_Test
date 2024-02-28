#version 430

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Color;

out vec3 ex_Color;

void main()
{
    gl_Position = vec4(in_Position.x * 0.5f, in_Position.y * 0.5f, in_Position.z, 1.0);
	ex_Color = in_Color ;
}