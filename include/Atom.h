#pragma once

#include "common.h"

class Atom
{
public:
    Atom(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec4 color = glm::vec4(0, 0, 0, 0)) 
        : position(position), color(color)
    {

    }
private:
    glm::vec3 position;
    glm::vec4 color;
};