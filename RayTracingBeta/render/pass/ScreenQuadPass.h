#pragma once
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"util\utils.h"
#include"render\Texture.h"
#include<iostream>
class ScreenQuadPass :public RenderPass
{
public:

	std::shared_ptr<Texture> lastFrame;

	ScreenQuadPass(std::string name, const char* vspath, const char* fspath) :RenderPass(name, vspath, fspath) {};

	virtual void init();
	virtual void update();
};