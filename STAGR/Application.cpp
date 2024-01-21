#include "Application.h"
#include <vector>
#include "Physics.h"
#include "Mouse.h"

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

    Time=0;
    DeltaTime=0.01;
    //currentTime=timeGetTime();
    Accumulator=0.0;

}

void Application::Run()
{
    m_Window->Update();
    Renderer::Get().ClearBuffer(0.2,0.2,0.2);
    if (m_Editor->spawnCall)
    {
        PhysicsObject Object(m_Editor->ui_size);

        Object.Model_ID = m_Editor->ModelType;

        Physics::all_sand.push_back(Object);

        Physics::all_sand.back().Spawn({ m_Editor->ui_xpos, m_Editor->ui_ypos });

        std::cout<<"Spawn call\n";
        m_Editor->spawnCall = false;
    }
    m_Editor->BuildEditorWindow();
    m_Editor->Render();
    Renderer::Get().DrawScene(Alpha);
    Renderer::Get().EndFrame();
}

void Application::Close()
{
    m_Window->Close();
}
