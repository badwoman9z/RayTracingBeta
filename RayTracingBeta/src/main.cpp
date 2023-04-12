#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include"Application.h"
#include"scene/Shape.h"
#include"util\utils.h"
#include"render\Buffer.h"
#include"render\RenderManager.h"
#include"render\pass\GbufferPass.h"
#include"render\pass\ScreenQuadPass.h"
#include"render\pass\RayTracingPass.h"
#include"render\pass\TAAPass.h"
#include"render\pass\GbufferPass.h"
// settings
const unsigned int SCR_WIDTH = 512;
const unsigned int SCR_HEIGHT = 512;
int main() {
    Application::camera = std::make_shared<Camera>(glm::vec3(0, 0, 0));


    Application::Init();
    if (Application::Create(SCR_WIDTH, SCR_HEIGHT) != 1) {
        glfwTerminate();
        return -1;
    }


    RenderManager::get_instance().loadModel("./model/Stanford Bunny.obj", 0.0, 0.0, getTransformMatrix(glm::vec3(0, 0, 0), glm::vec3(1.5, -0.3, -3.5), glm::vec3(5, 5,5)), false);
    RenderManager::get_instance().loadModel("./model/sphere2.obj", 1.0, 1.0, getTransformMatrix(glm::vec3(0, 0, 0), glm::vec3(2.0,0.5, -2.5), glm::vec3(0.6, 0.6, 0.6)), true);
    RenderManager::get_instance().loadModel("./model/quad.obj", 2.0, 2.0, getTransformMatrix(glm::vec3(0, 0, 0), glm::vec3(0.0, -0.2, 0), glm::vec3(10.83, 0.01, 10.83)), true);

    //RenderManager::get_instance().loadModel("./model/floor.obj", 1.0, 1.0, getTransformMatrix(glm::vec3(0, 0, 0), glm::vec3(0, -0.2, -2), glm::vec3(1.0, 1.0, 1.0)), false);

    //RenderManager::get_instance().loadModel("./model/floor.obj", 2.0, 2.0, getTransformMatrix(glm::vec3(0, 0, 0), glm::vec3(0, 0.8, -2.5), glm::vec3(0.01, 1.0, 0.01)), false);


    RenderManager::get_instance().regisiterCamera(Application::camera);

    RenderManager::get_instance().regisiterRenderPass(std::make_shared<GbufferPass>("GbufferPass", "./shader/GbufferPass.vs", "./shader/GbufferPass.fs"));

    RenderManager::get_instance().regisiterRenderPass(std::make_shared<RayTracingPass>("RayTracingPass", "./shader/raytracing.comp"));
    RenderManager::get_instance().regisiterRenderPass(std::make_shared<TAAPass>("TAAPass", "./shader/TAAPass.comp"));
    
    RenderManager::get_instance().regisiterRenderPass(std::make_shared<ScreenQuadPass>("ScreenQuad", "./shader/ScreenQuadPass.vs", "./shader/ScreenQuadPass.fs"));



    RenderManager::get_instance().init();

    Application::frameCounter = RenderManager::get_instance().frameCount;



    // render loop
// -----------
    while (!glfwWindowShouldClose(Application::window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        Application::deltaTime = currentFrame - Application::lastFrame;
        Application::lastFrame = currentFrame;
        Application::processInput(Application::window);

        RenderManager::get_instance().update();



        Application::Update();


    }

    glfwTerminate();

    return 0;

}