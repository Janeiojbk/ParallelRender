#pragma once

#include "common.h"
#include "OpenGLApp.h"
#include "Camera.h"

#include <mpi.h>

class OpenGLApp;

class InputProcessor
{
public:
    static InputProcessor *Instance() {
        if (instance == nullptr) {
            instance = new InputProcessor();
        }
        return instance;
    }

    virtual void Initialize(OpenGLApp *app) {
        this->app = app;
        BindCallback();
    }

    virtual void ProcessInput();
protected:
    virtual void BindCallback()
    {

    }
private:
    InputProcessor() 
    { 

    }

    void BroadCastPosition() {
        MPI_Barrier(MPI_COMM_WORLD);
    }


    OpenGLApp *app;
    static InputProcessor *instance;
};