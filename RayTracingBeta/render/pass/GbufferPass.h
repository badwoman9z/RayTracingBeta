#pragma once
#include"render/RenderPass.h"
#include"render\Texture.h"
#include"render\RenderManager.h"
#include<GL/glew.h>
#include<vector>
class GbufferPass :public RenderPass
{
public:
	unsigned int FBO;
	unsigned int RBO;
	unsigned int VAO;

	std::shared_ptr<Texture> normalWithMaterial;
	std::shared_ptr<Texture> positionWithDepth;

	GbufferPass(std::string name, const char* vspath, const char* fspath) :RenderPass(name, vspath, fspath) {};

	glm::mat4 projection;
	
	virtual void init();
	virtual void update();

};