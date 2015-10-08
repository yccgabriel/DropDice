#pragma once
#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#define _CRT_SECURE_NO_DEPRECATE	// for the troublesome UNSAFE fopen

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SOIL.h>
#include <stdio.h>
#include <iostream>

class ShaderManager
{
// Singleton
private:
	static bool _instanceFlag;
	static ShaderManager *single;
	ShaderManager();
public:
	static ShaderManager* GetInstance();
	~ShaderManager();
// end of Singleton
public:
	struct{
		GLuint	textures[6];
		GLuint	vertexShader, fragmentShader;
		GLuint	shaderProgram;
		struct{
			GLuint overrideColor;
			GLuint model;
			GLuint view;
			GLuint proj;

			GLuint useTexture;
			GLuint texture;
		}uniforms;
		struct{
			GLint position;
			GLint color;
			GLint texcoord;
		}attribs;
	}resources;

	void	DefineAttribs();
	void	ActivateProgram();
	void	UseTexture(GLuint);
private:
	GLuint	MakeShader(GLenum, const char*);
	GLuint	MakeProgram(GLuint, GLuint);
	GLuint	MakeTexture(const char *);
	void	DefineUniforms();
	void	DefineOuts();
	void*	FileContents(const char*, GLint*);
	void	ShowInfoLog(GLuint, PFNGLGETSHADERIVPROC, PFNGLGETSHADERINFOLOGPROC);
};

#endif // SHADERMANAGER_H