#include"TAAPass.h"

void TAAPass::init()
{
	filteredFrame = std::make_shared<Texture>(GL_RGBA32F, GL_FLOAT, GL_RGBA, RenderManager::get_instance().getWidth(), RenderManager::get_instance().getHeight());

	RenderManager::get_instance().regisiterTexture("filteredFrame", filteredFrame->ID);

	


}

void TAAPass::update()
{

	shader->use();
	shader->setBool("isFirstFrame", isFirstFrame);
	shader->setFloat("frameCounter", RenderManager::get_instance().getFrameCount());
	glBindImageTexture(5, filteredFrame->ID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	//std::cout << RenderManager::get_instance().getFrameCount() << std::endl;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("lastFrame"));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("frameTexture"));
	shader->setInt("lastFrame", 0);
	shader->setInt("frameTexture", 1);
	shader->setInt("gbufferNormal", 2);
	shader->setInt("gbufferPosition", 3);
	shader->setMat4("lastViewMat", RenderManager::get_instance().getLastCameraView());
	shader->setMat4("projection", RenderManager::get_instance().getProjectionMat());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("gbufferNormal"));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("gbufferPosition"));



	glDispatchCompute((unsigned int)32, (unsigned int)32, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
	isFirstFrame = false;
}
