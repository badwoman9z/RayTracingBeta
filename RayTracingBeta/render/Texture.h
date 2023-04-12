#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>


class Texture
{
public:
	unsigned int ID;
	std::string path;
	std::string name;
	unsigned char* bytes = nullptr;
	float* data = nullptr;
	int width, height, numColCh;


	Texture(GLenum internalFormat, GLenum type, GLenum format, int w, int h);


private:

};