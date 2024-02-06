#include "Application.h"
#include <vector>
#include "Physics.h"
#include "Mouse.h"
#include "WinInclude.h"

std::vector<PhysicsObject> Physics::all_sand;
int PhysicsObject::nxt_id = 0;
float Mouse::lastX = 400;
float Mouse::lastY = 400;
bool Mouse::first = true;
bool Mouse::enabled = false;



void Application::Init()
{
    m_Window=&Window::Get();
    m_Window->Init();
    Log("Window initialized");
    

    Renderer::Get().InitScene();
    Log("Renderer initialized");
    Log("Scene initialized");
    m_Editor= new Editor(*m_Window);
    Log("Editor initialized");
    m_Window->SetDeviceContextInitialized(true);

    Time=0.0f;
    DeltaTime=0.01;
    m_currentTime=timeGetTime();
    Accumulator=0.0;

}

void Application::Run()
{
   
    auto new_time = (timeGetTime()); //<--- fix it
    auto frame_time = new_time - m_currentTime;
    if (frame_time > 0.25)
        frame_time= 0.25;

    m_currentTime = new_time;
    m_Window->Update();
    Renderer::Get().ClearBuffer(0.2,0.2,0.2);
    //Log(frame_time);
    Accumulator += frame_time;
    if (m_Editor->spawnCall)
    {
        PhysicsObject Object(m_Editor->ui_size);

        Object.Model_ID = m_Editor->ModelType;

        Physics::all_sand.push_back(Object);

        Physics::all_sand.back().Spawn({ m_Editor->ui_xpos, m_Editor->ui_ypos });

        m_Editor->spawnCall = false;
    }
    while (Accumulator >= DeltaTime)
    {
        Physics::previousToCurrent();
        Time += DeltaTime;
        InputManager::Get().HandleInput(DeltaTime); 
        Accumulator -= DeltaTime;
    }
    
    Alpha = Accumulator / DeltaTime;
    m_Editor->BuildEditorWindow();
    m_Editor->Render();
    Renderer::Get().DrawScene(Alpha);
    Renderer::Get().EndFrame();
}

void Application::Close()
{
    m_Window->Close();
}
