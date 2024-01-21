#pragma once
#include "WinInclude.h"
class InputManager
{
    /*Renderer(const Renderer&) = delete;
    Renderer& operator = (const Renderer&) = delete;

    inline static Renderer& Get()
    {
        static Renderer instance;
        return instance;
    }*/
public:

    InputManager(const InputManager&) = delete;
    InputManager& operator = (const InputManager&) = delete;

    inline static InputManager& Get() 
    {
        static InputManager instance;
        return instance;
    }
    void OnKeyPressed(UINT8);
    InputManager() = default;
};

