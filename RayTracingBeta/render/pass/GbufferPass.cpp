#include"GbufferPass.h"

void GbufferPass::init()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, RenderManager::get_instance().ssbo_triangles->getBufferID());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec4), (void*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec4), (void*)(4*sizeof(float)));

	glBindVertexArray(0);
	
	normalWithMaterial = std::make_shared<Texture>(GL_RGBA32F, GL_FLOAT, GL_RGBA, RenderManager::get_instance().getWidth(), RenderManager::get_instance().getHeight());
	positionWithDepth = std::make_shared<Texture>(GL_RGBA32F, GL_FLOAT, GL_RGBA, RenderManager::get_instance().getWidth(), RenderManager::get_instance().getHeight());

	std::cout << normalWithMaterial->ID << std::endl;
	std::cout << positionWithDepth->ID << std::endl;
	RenderManager::get_instance().regisiterTexture("gbufferNormal", normalWithMaterial->ID);
	RenderManager::get_instance().regisiterTexture("gbufferPosition", positionWithDepth->ID);

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionWithDepth->ID, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalWithMaterial->ID, 0);
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	
	glDrawBuffers(2, attachments);
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, RenderManager::get_instance().getWidth(), RenderManager::get_instance().getHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	
	
}

void GbufferPass::update()
{
	glEnable(GL_DEPTH_TEST);
	shader->use();
	
	
	glBindFramebuffer(GL_FRAMEBUFFER,FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glViewport(0, 0, RenderManager::get_instance().getWidth(), RenderManager::get_instance().getHeight());

	shader->setMat4("view", RenderManager::get_instance().camera->GetViewMatrix());
	shader->setMat4("projection", RenderManager::get_instance().getProjectionMat());
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, RenderManager::get_instance().triangles_encoded.size() * 3);

	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
