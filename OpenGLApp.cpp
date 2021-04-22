#include "OpenGLApp.h"

#include <imgui/imgui.h>
#include <json.h>
#include "imgui/imgui_impl_glfw_gl3.h"

#include "MyUtil.h"

using json = nlohmann::json;

OpenGLApp *OpenGLApp::instance = nullptr;

void OpenGLApp::Initialize()
{
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    int size = sqrt(world_size);

    ifstream in;
    in.open("./Resources/config.json");
    if(!in.is_open()) {
        cerr << "Config file open failed";
    }
    json config;
    in >> config;
    ScreenWidth = config["width"] / size;
    ScreenHeight = config["height"] / size;

    pWindow = glfwCreateWindow(ScreenWidth, ScreenHeight, AppName.c_str(), NULL, NULL);
    if(pWindow == nullptr) {
        cout << "Window Create Failed" << endl;
        glfwTerminate();
        throw std::runtime_error("Window create failed");
    }

    int offsetX = world_rank % size;
    int offsetY = world_rank / size;

    int locationX = offsetX * ScreenWidth, locationY = ScreenHeight * offsetY;
    glfwSetWindowPos(pWindow, locationX, locationY);

    glfwMakeContextCurrent(pWindow);
    glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }   

    // configure global opengl state
    // -----------------------------
    // glEnable(GL_DEPTH_TEST);

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(pWindow, true);

    inputProcessor = InputProcessor::Instance();
    inputProcessor->Initialize(OpenGLApp::Instance());
}

void OpenGLApp::Update()
{
    // input
    // -----
    glfwPollEvents();
    inputProcessor->ProcessInput();
    isExit = glfwWindowShouldClose(pWindow);
}


void OpenGLApp::Tick()
{
    lastFrameTime = currentFrameTime;
    currentFrameTime = glfwGetTime();
}

void ShowFPS() 
{
    ImGui::SetNextWindowPos(ImVec2(OpenGLApp::Instance()->ScreenWidth - 90, 10));
    ImGui::SetNextWindowSize(ImVec2(80, 30));
    bool pOpen = true;
    if (!ImGui::Begin("Fixed Overlay", &pOpen, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("Screen:%d", OpenGLApp::Instance()->world_rank);
    ImGui::End();
}

void OpenGLApp::Draw() 
{
    ImGui_ImplGlfwGL3_NewFrame();

    ShowFPS();

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->use();

    glm::mat4 view = camera.GetViewMatrix();
    shader->setMat4("view", view);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    glm::mat4 tileProjection = projection;
    

    int size = sqrt(world_size);
    tileProjection[0][0] *= size;
    tileProjection[1][1] *= size;
    tileProjection[2][0] = -size + 2 * (world_rank % size) + 1;
    tileProjection[2][1] = size - 2 * (world_rank / size) + 1;

    shader->setMat4("projection", tileProjection);

    for(auto &object : drawableList) {
        object->Draw(*shader);
    }

    ImGui::Render();

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(pWindow);
    glfwPollEvents();
}

void OpenGLApp::Start()
{
    shader = new Shader("./Resources/Shader/Simple/simple.vs", "./Resources/Shader/Simple/simple.fs");
    Model *model = new Model("Resources/Model/nanosuit.obj");
    drawableList.push_back(model);
}

void OpenGLApp::Finalize() 
{
    glfwTerminate();
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    OpenGLApp *instance = OpenGLApp::Instance();
    instance->ScreenWidth = width;
    instance->ScreenHeight = height;
    instance->OnWindowSizeChanged();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);