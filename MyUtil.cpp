#include "MyUtil.h"

#include <iostream>

void PrintMat4(glm::mat4 mat4)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << mat4[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "----------------------------" << std::endl;
}

void PrintVec3(glm::vec3 vec3)
{
    for (int i = 0; i < 3; i++) {
        std::cout << vec3[i] << " ";
    }
    std::cout << std::endl;
}