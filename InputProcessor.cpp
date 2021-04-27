#include "InputProcessor.h"

InputProcessor *InputProcessor::instance = nullptr;

void InputProcessor::ProcessInput()
{
    Camera &camera = app->GetCurrentCamera();
    GLFWwindow *window = app->GetCurrentWindow();
    double deltaTime = app->GetDeltaTime();



    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    Camera_Movement operation = NONE;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        operation = FORWARD;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        operation = BACKWARD;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        operation = LEFT;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        operation = RIGHT;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        operation = UP;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        operation = DOWN;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        operation = TURN_UP;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        operation = TURN_DOWN;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        operation = TURN_LEFT;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        operation = TURN_RIGHT;
    }

    // Gather all partial averages down to the root process
    Camera_Movement *operations = NULL;
    if (app->world_rank == 0) {
        operations = (Camera_Movement *)malloc(sizeof(Camera_Movement) * app->world_size);
    }
    MPI_Gather(&operation, 1, MPI_INT, operations, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (app->world_rank == 0) {
        for(int i = 0; i < app->world_size; i++) {
            camera.ProcessKeyboard(operations[i], deltaTime);
        }
    }

    MPI_Bcast(&camera.Position[0], 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}