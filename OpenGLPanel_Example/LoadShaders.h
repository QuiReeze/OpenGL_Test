#pragma once
#include <GL/glew.h>
#include <iostream>
typedef struct
{
	GLenum type;
	const char* filename;
	GLuint       shader;
}ShaderInfo;

const GLchar* ReadShader(const char* filename);
GLuint LoadShaders(ShaderInfo*);
