#pragma once

#include<soil2/SOIL2.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <iostream> 
#include<fstream>
#include<string>
using namespace std;

void printShaderLog(GLuint shader)
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}

void printProgramLog(GLuint prog)
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}

bool checkOpenGLError()
{
	bool founderr = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		cout << "glError: " << glErr << endl;
		founderr = true;
		glErr = glGetError();
	}
	return founderr;
}

string readShaderSource(const char* filepath)
{
	string content;
	ifstream filestream(filepath, ios::in);
	string line = "";
	while (!filestream.eof())
	{
		getline(filestream, line);
		content.append(line + "\n");
	}
	filestream.close();
	return content;
}

GLuint createShaderProgram(const char* vp, const char* fp) /*create shader by reading files*/
{
	string vertshaderstr = readShaderSource(vp);
	string fragshaderstr = readShaderSource(fp);


	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* vertshadersrc = vertshaderstr.c_str();
	const char* fragshadersrc = fragshaderstr.c_str();
	glShaderSource(vshader, 1, &vertshadersrc, NULL);
	glShaderSource(fshader, 1, &fragshadersrc, NULL);


	GLint vertcompiled;
	GLint fragcompiled;
	GLint linked;
	/*catch errors while compiling shaders*/
	glCompileShader(vshader);
	checkOpenGLError();
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &vertcompiled);
	if (vertcompiled != 1)
	{
		cout << "vertex compilation failed!" << endl;
		printShaderLog(vshader);
	}

	glCompileShader(fshader);
	checkOpenGLError();
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &fragcompiled);
	if (fragcompiled != 1)
	{
		cout << "fragment compilation failed!" << endl;
		printShaderLog(fshader);
	}

	/*catch erros while linking shaders*/
	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vshader);
	glAttachShader(vfProgram, fshader);
	glLinkProgram(vfProgram);
	checkOpenGLError();
	/*in this program glGetErr returns error code 1280, but discussion on stackoverflow says:
	"in some cases you may still get GL_INVALID_ENUM after specifying glewExperimental depending
	on your glew version".It sounds like it might be safe to ignore as long as you're not seeing any other problems.*/
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1)
	{
		cout << "linking failed!" << endl;
		printProgramLog(vfProgram);
	}

	return vfProgram;
}


GLuint loadTexture(const char* texImagePath)
{
	GLuint textureID;
	textureID = SOIL_load_OGL_texture(texImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (textureID == 0) cout << "could not find texture file" << texImagePath << endl;
	return textureID;
}