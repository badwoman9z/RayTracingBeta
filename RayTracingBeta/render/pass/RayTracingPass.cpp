#include "RayTracingPass.h"
#include <random> // necessary for generation of random floats (for sample kernel and noise texture)
void RayTracingPass::init()


{
	HDRLoaderResult hdrRes;
	bool r = HDRLoader::load("./HDR/peppermint_powerplant_4k.hdr", hdrRes);
	hdrTexture = std::make_shared<Texture>(GL_RGB32F, GL_FLOAT, GL_RGB, hdrRes.width, hdrRes.height);
	glBindTexture(GL_TEXTURE_2D, hdrTexture->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, hdrRes.width, hdrRes.height, 0, GL_RGB, GL_FLOAT, hdrRes.cols);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// hdr 重要性采样 cache
	hdrCache = std::make_shared<Texture>(GL_RGB32F, GL_FLOAT, GL_RGB, hdrRes.width, hdrRes.height);
	glBindTexture(GL_TEXTURE_2D, hdrCache->ID);
	float* cache = calculateHdrCache(hdrRes.cols, hdrRes.width, hdrRes.height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, hdrRes.width, hdrRes.height, 0, GL_RGB, GL_FLOAT, cache);
	glBindTexture(GL_TEXTURE_2D, 0);
	std::cout << hdrRes.width<<hdrRes.height << std::endl;
	std::cout << "hdr finished" << std::endl;
	frameTexture = std::make_shared<Texture>(GL_RGBA32F, GL_FLOAT, GL_RGBA, RenderManager::get_instance().getWidth(), RenderManager::get_instance().getHeight());

	glBindImageTexture(7, frameTexture->ID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	RenderManager::get_instance().regisiterTexture("frameTexture", frameTexture->ID);

	shader->use();
	shader->setInt("hdrResolution", hdrRes.width);

}

void RayTracingPass::update()
{
	//glClearTexImage(frameTexture->ID, 0, GL_RGBA, GL_FLOAT, NULL);
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // 随机浮点数，范围0.0 - 1.0
	std::default_random_engine generator;

	shader->use();
	shader->setFloat("frameCounter", RenderManager::get_instance().getFrameCount());
	shader->setInt("gbufferPosition", 0);
	shader->setInt("gbufferNormal", 1);
	shader->setInt("hdrMap", 2);
	shader->setInt("hdrCache", 3);
	shader->setMat4("u_view",RenderManager::get_instance().camera->GetViewMatrix());
	shader->setMat4("u_projection", RenderManager::get_instance().getProjectionMat());
	shader->setVec3("eye", RenderManager::get_instance().camera->Position);
	for (int i = 0; i < 4; i++) {
		shader->setFloat("rdSeed[" + std::to_string(i) + "]", randomFloats(generator));
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("gbufferNormal"));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("gbufferPosition"));	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, hdrTexture->ID);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, hdrCache->ID);
	glDispatchCompute((unsigned int)32, (unsigned int)32, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
	
}