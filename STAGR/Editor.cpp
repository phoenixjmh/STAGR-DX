#include "Editor.h"

#include <iostream>
#include <string>
#include "Renderer.h"
#include "Physics.h"



float Renderer::lightPosX = 1;
float Renderer::lightPosY = 1;
float Renderer::lightPosZ=1;




void Editor::init_imgui(const Window& window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(window.GetWindow());
    ImGui_ImplDX11_Init(Renderer::Get().GetDevice().Get(),Renderer::Get().GetContext().Get() );
}

void Editor::AddObjectTransformEditor(unsigned int index)
{

    float* xpos = &Physics::all_sand[index].editor_pos.x;
    float* ypos = &Physics::all_sand[index].editor_pos.y;
    float* zpos = &Physics::all_sand[index].editor_pos.z;

    float* size = &Physics::all_sand[index].editor_size;

    std::string str_x = "X: " + std::to_string(index);
    std::string str_y = "Y: " + std::to_string(index);
    std::string str_z = "Z: " + std::to_string(index);
    std::string str_size = "Size: " + std::to_string(index);
    
    ImGui::SliderFloat(str_x.c_str(), xpos, -30, 30);

    ImGui::SliderFloat(str_y.c_str(), ypos, -30, 30);

    ImGui::SliderFloat(str_z.c_str(), zpos, -30, 30);

    ImGui::SliderFloat(str_size.c_str(), size, 0.1, 2);

}
void Editor::DisplayModelSwitcher()
{
    const char* const options[] = { "sphere", "backpack", "cube","floor" };
    int size = sizeof(options) / sizeof(const char*);
    // Call ListBox and update the member variable
    if (ImGui::ListBox("Model:", &ModelType, ModelTypeGetter,
        (void*)options, size, 4))
    {
        // ListBox value changed, update the member variable
        ModelType = ModelType;
    }
}
std::string modelIDToString(int id)
{
    std::string name;
    switch (id)
    {
    case 0:
        name = "Sphere";
        return name;
    case 1:
        name = "Backpack";
        return name;
    case 2:
        name = "Cube";
        return name;
    case 3:
        name = "Floor";
        return name;



    }
} 
void Editor::BuildEditorWindow()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    static float f = 0.0f;
    static int counter = 0;

    {
        ImGui::Begin("Scene Hierarchy");

        ImGui::Checkbox("Simulate", &debug_is_simulate);

        ImGui::Checkbox("Camera Control", &camera_input);

        ImGui::Checkbox("Render Lighting", &renderer_lighting);

        if (ImGui::Button("Spawn Object"))
        {
            spawnCall = true;
        }

        ImGui::SameLine();
        DisplayModelSwitcher();


        for (int i = 0; i < Physics::all_sand.size(); i++)
        {
            unsigned int ModelID = Physics::all_sand[i].Model_ID;

            std::string name = modelIDToString(ModelID);

            ImGui::SeparatorText(name.c_str());

            AddObjectTransformEditor(i);

            ImGui::Dummy(ImVec2(0.0f, 20.0f));
        }
        ImGui::End();
    }


    {
        ImGui::Begin("Engine Stats");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
        ImGui::End();

    }

    {
        auto* camPos = &Renderer::Get().GetCamera(). camera_position;
        auto * camFront =&Renderer::Get().GetCamera(). camera_front;
        auto* camTarget = &Renderer::Get().GetCamera(). camera_target;
        auto *camDir = &Renderer::Get().GetCamera(). camera_direction;
        
        Renderer::Get().GetCamera(). camera_right;
        ImGui::Begin("Camera:");
        ImGui::SliderFloat3("Camera Position:", glm::value_ptr(*camPos), -3, 3);
        ImGui::SliderFloat3("Camera Front:", glm::value_ptr(*camFront), -3, 3);
        ImGui::SliderFloat3("Camera Target:", glm::value_ptr(*camTarget), -3, 3);
        ImGui::SliderFloat3("Camera Direction:", glm::value_ptr(*camDir),-3,3);
        ImGui::End();
    }

    {
        float* light_posx = &Renderer::Get().GetSceneData().light_data.light_position.x;
        float* light_posy = &Renderer::Get().GetSceneData().light_data.light_position.y;
        float* light_posz = &Renderer::Get().GetSceneData().light_data.light_position.z;

        ImGui::Begin("Debug Light Pos");
        ImGui::SliderFloat("Light x", light_posx, -20, 20);
        ImGui::SliderFloat("Light y", light_posy, -20, 20);
        ImGui::SliderFloat("Light z",light_posz,-20,20);
        ImGui::End();
    }
    ImGui::Render();

}

void Editor::Render()
{
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


void Editor::Shutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

bool Editor::ModelTypeGetter(void* data, int idx, const char** out_text)
{
    // Assuming data is an array of string labels
    const char* const* items = static_cast<const char* const*>(data);
    if (out_text)
    {
        *out_text = items[idx];
        return true;
    }
    return false;
}

