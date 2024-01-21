#pragma once
//#include "Physics.h"
//#include "Mouse.h"
#include "Window.h"


class Editor {
public:
    Editor(const Window& window) :
        ui_xpos(0),
        ui_ypos(0),
        ui_size(1),
        debug_is_simulate(false),
        camera_input(false),
        spawnCall(false)
    {
        init_imgui(window);
    }
    void BuildEditorWindow();
    void Render();
    void Shutdown();

   /* void BuildEditorWindow();
    void Render();
    void Shutdown(); */
    int ModelType = 0;
    float ui_xpos;
    float ui_ypos;
    float ui_zpos;
    float ui_size;



    bool debug_is_simulate;
    bool camera_input;
    bool renderer_lighting;
    ImGuiIO* io;
    bool spawnCall;
    
private:
    void DisplayModelSwitcher();
    void AddObjectTransformEditor(unsigned int  index);
    static bool ModelTypeGetter(void* data, int idx, const char** out_text);
    void init_imgui(const Window& window);
};


