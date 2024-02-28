#pragma once

#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

class ParamGL
{
public:

	// Vertex array object
	GLuint VAO;

	// Vertex buffer object
	GLuint VBOv;

	// Color buffer object
	GLuint VBOc;

	// Shader program
	GLuint Program;
};

struct Vec3f
{
	float x;
	float y;
	float z;
};