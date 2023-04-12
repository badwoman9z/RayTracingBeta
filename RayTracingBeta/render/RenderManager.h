#pragma once
#include"RenderPass.h"
#include<vector>
#include <unordered_map>
#include <glm\gtx\string_cast.hpp>
#include<initializer_list>
#include <algorithm>
#include"util\utils.h"
#include"Singleton.h"
#include"scene/Shape.h"
#include"render\Buffer.h"
#include"scene\Camera.h"
#include <omp.h>    // openmp多线程加速
class RenderManager :public Singleton<RenderManager>
{

	friend class Singleton<RenderManager>;

public:
	RenderManager(const RenderManager&) = delete;
	RenderManager& operator=(const RenderManager&) = delete;

private:
	RenderManager() = default;

	int width = 512;
	int height = 512;
	

	glm::mat4 lastViewMat = glm::mat4(1.0);
	glm::mat4 projection;
public:

	std::shared_ptr<float> frameCount;
	std::vector<std::shared_ptr<RenderPass>> renderPipeLine;
	std::unordered_map<std::string, unsigned int> textureMap;
	
	//场景
	std::vector<Triangle> triangles;
	std::vector<BVHNode> bvhnodes;
	std::vector<Material> materials;

	std::vector<SSBO_Triangle> triangles_encoded;
	std::vector<SSBO_BVHNode> bvhnodes_encoded;
	std::shared_ptr<Camera> camera;


	//shader 变量
	std::shared_ptr<Buffer> ssbo_triangles;
	std::shared_ptr<Buffer> ssbo_bvhnodes;
	std::shared_ptr<Buffer> ssbo_material;



	GLint ScreenQuadVAO = 0;
	
	
	
	
	
	
	void regisiterCamera(std::shared_ptr<Camera> c) {
		camera = c;
	}
	int getFrameCount() {
		return *frameCount;
	}
	int getWidth() {
		return this->width;
	}
	int getHeight() {
		return this->height;
	}
	
	void reSetFrameCount() {
		frameCount = 0;
	}
	
	void loadModel(std::string filepath, float materialID, float meshID, glm::mat4 trans, bool smoothNormal);
	int BuildBVHTree(int left, int right, int n);
	void EncodeSSBOTriangle();
	void EncodeSSBOBVH();

	void regisiterRenderPass(std::shared_ptr<RenderPass> pass);
	void init();
	void update();

	glm::vec3 getCameraPos() {
		return camera->Position;
	}
	glm::mat4 getCameraRotate() {
		return camera->GetInverseViewMatrix();
	}
	glm::mat4 getLastCameraView() {
		return lastViewMat;
	}
	glm::mat4 getProjectionMat() {
		return projection;
	}
	GLint getOrCreateScreenQuadVAO();	
	void regisiterTexture(std::string s, unsigned int textureId);
	unsigned int getTexture(std::string s);


};

