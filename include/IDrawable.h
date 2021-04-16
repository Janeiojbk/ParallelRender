#pragma once

#include "common.h"
#include "ShaderHelper.h"

interface IDrawable
{
public:
    virtual void Draw(Shader &shader) = 0;
};