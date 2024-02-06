#pragma once
#include "WinInclude.h"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_dx11.h"
#include "vendor/imgui/imgui_impl_win32.h"
#include <vector>


class Window
{
public:
    void Init();
    void Update();
    void Close();

    inline bool ShouldClose() { return m_shouldClose; }

    HWND  GetWindow() const& { return m_window; }

    inline static Window& Get()
    {
        static Window instance;
        return instance;
    }
    //Keyboard kbd

    void SetDeviceContextInitialized(bool b) { IsContextInitialized = b; }

    
private:

    Window() = default;

    bool m_shouldClose = false;

    bool IsContextInitialized = false;

    HWND m_window = nullptr;

    ATOM m_windowClass;

    std::vector<BYTE> rawBuffer;

    bool MouseListen = false;

    bool RawInputListen = false;


public:

    static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    static LRESULT HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

    void EnableCursor();

    void DisableCursor(const HWND& hWnd);

    void LockCursorToCenter();

    void DisableRawInput();

    void EnableRawInput();

};

