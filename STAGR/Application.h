#pragma once
#include "Editor.h"
//#include "Mouse.h"
//#include "Physics.h"
//#include "Renderer.h"
#include "Renderer.h"
#include "InputManager.h"


class Application {

public: Application()
    :
    m_Editor(nullptr),
    //m_Renderer(nullptr),
    m_Window(nullptr)
{
    Init();
}
      void Init();
      void Run();
      void Close();

      Window* GetWindow(){return m_Window;}

private:

    Window* m_Window;
    /*Physics m_Field;
    Renderer* m_Renderer;*/
    Editor* m_Editor;


    //void ProcessInput();

    // Time logic

    double currentTime;
    double Time;
    double DeltaTime;
    double Alpha;
    double Accumulator;

    //Mouse mouse;

    float yaw = -90.f;
    float pitch;
};

