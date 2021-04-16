#pragma once

#include "common.h"
#include "IApp.h"
#include "ShaderHelper.h"
#include "IDrawable.h"
#include "Mesh.h"
#include "Camera.h"
#include "InputProcessor.h"
#include "Model.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <chrono>
#include <mpi.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// I do not wanna abstract this now, so input event is depended by glfw now~
void processInput(GLFWwindow *window);

class InputProcessor;

class OpenGLApp implements IApp 
{
public:
    static OpenGLApp *Instance() {
        if (instance == nullptr) {
            instance = new OpenGLApp();
        }
        return instance;
    }
    virtual void Initialize() override;
    virtual void Finalize() override;

    Camera &GetCurrentCamera() {
        return camera;
    }

    GLFWwindow *GetCurrentWindow() {
        return pWindow;
    }

    double GetDeltaTime() {
        return currentFrameTime - lastFrameTime;
    }

    int world_rank;
    int world_size;
protected:
    OpenGLApp() : IApp() 
    {

    }
    virtual void Update() override;
    virtual void Draw() override;
    virtual void Start() override;
    virtual void Tick() override;

    Shader *shader;

    std::vector<IDrawable *> drawableList;

    Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)ScreenWidth / (float)ScreenHeight, 0.1f, 100.0f);

    static OpenGLApp *instance;
    
    GLFWwindow *pWindow;
    InputProcessor *inputProcessor;

    double lastFrameTime, currentFrameTime;
};