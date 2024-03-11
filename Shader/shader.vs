#version 430

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Color;

uniform mat4 rotationMatrix;
uniform mat4 moveMatrix;
uniform mat4 lookMatrix;
uniform mat4 projectMatrix;

out vec3 ex_Color;

void main()
{
	gl_Position =  projectMatrix * lookMatrix * moveMatrix * rotationMatrix * vec4(in_Position.x * 0.5f, in_Position.y * 0.5f, in_Position.z * 0.5f, 1.0);
	ex_Color = in_Color ;
}