
#pragma once
#include"render/RenderPass.h"
#include<GL/glew.h>
#include<vector>
#include"render\RenderManager.h"
#include"scene/Shape.h"
#include"util\hdrloader.h"
#include"render\Texture.h"
#include"util\utils.h"
class RayTracingPass :public RenderPass
{
public:


	std::shared_ptr<Texture>  frameTexture;	
	std::shared_ptr<Texture> hdrTexture;
	std::shared_ptr<Texture> hdrCache;
	RayTracingPass(std::string name, const char* cspath) : RenderPass(name, cspath) {};
	int frameCount=10;
	virtual void init();
	virtual void update();

};
