#include"ScreenQuadPass.h"
void ScreenQuadPass::init() {
	lastFrame = std::make_shared<Texture>(GL_RGBA32F, GL_FLOAT, GL_RGBA, RenderManager::get_instance().getWidth(), RenderManager::get_instance().getHeight());

	glBindImageTexture(6, lastFrame->ID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	RenderManager::get_instance().regisiterTexture("lastFrame", lastFrame->ID);
	
	
}


void ScreenQuadPass::update() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, RenderManager::get_instance().getWidth(), RenderManager::get_instance().getHeight());
	shader->use();
	shader->setInt("filteredFrame", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("filteredFrame"));
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}