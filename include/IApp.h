#pragma once

#include "common.h"

#include <string>

interface IApp 
{
public:
    virtual void Initialize() = 0;

    virtual void Run() 
    {
        Start();
        while (!isExit)
        {
            Tick();
            Update();
            Draw();
        }
        Finalize();
    }

    virtual void OnWindowSizeChanged()
    {
        return;
    }

    virtual void ProcessInput()
    {
        return;
    }
    
    IApp(IApp &rhs) = delete;
    IApp &operator=(IApp &rhs) = delete;

    std::string AppName = "";
    int ScreenWidth = 100;
    int ScreenHeight = 100;
protected:
    IApp() {

    }

    virtual void Start()
    {
        return;
    }
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Tick()
    {
        return;
    }
    virtual void Finalize() 
    {
        return;
    }
    bool isExit = false;
};