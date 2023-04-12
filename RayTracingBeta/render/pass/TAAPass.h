#pragma once
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"util\utils.h"
#include"render\Texture.h"
#include<iostream>
class TAAPass:public RenderPass
{
public:

	std::shared_ptr<Texture> filteredFrame;


	TAAPass(std::string name, const char* cspath) : RenderPass(name, cspath) {};

	virtual void  init();
	virtual void update();

	bool isFirstFrame = true;

};
