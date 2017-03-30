#pragma once
#include <GL\glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader
{
public:
	GLuint Program;
	Shader(const GLchar*, const GLchar*);
	void USE();
};


