#include "Window.h"
#include "InputManager.h"

#define THROW_HRESULT(hrcall) if( FAILED( hr = (hrcall) ) ) std::cerr<< __LINE__<<__FILE__<<hr <<"\n"
void Window::Init()
{

    HRESULT hr;
    WNDCLASSEXW wcex(
        sizeof(WNDCLASSEXW)
        , CS_OWNDC
        , &Window::HandleMessageSetup
        , 0
        , 0
        , GetModuleHandle(nullptr)
        , LoadIconW(nullptr, IDI_APPLICATION)
        , LoadCursorW(nullptr, IDC_ARROW)
        , nullptr
        , nullptr
        , L"D3D12ExWndCls"
        , LoadIconW(nullptr, IDI_APPLICATION)
    );
    m_windowClass = RegisterClassExW(&wcex);
    if (m_windowClass == 0)
    {
        std::cerr << "Failed to create window!\n";
    }
    m_window = CreateWindowExW(
        WS_EX_OVERLAPPEDWINDOW | WS_EX_APPWINDOW,
        (LPCWSTR)m_windowClass, L"STAGR",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0, 1280, 720,
        nullptr, nullptr, wcex.hInstance,
        this);

    if (m_window == nullptr)
    {
        std::cerr << GetLastError() << "\n";
    }

    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
    rid.dwFlags = 0;
    rid.hwndTarget = nullptr;
    if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
    {
        MessageBoxA(m_window, "Failed to create Raw input device for mouse", "INPUT DEVICE ERROR", MB_ABORTRETRYIGNORE);
    }

}
void Window::Update()
{
    MSG msg;
    while (PeekMessageW(&msg, m_window, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

void Window::Close()
{
    if (m_windowClass)
    {
        DestroyWindow(m_window);
        UnregisterClassW((LPCWSTR)m_windowClass, GetModuleHandle(nullptr));
    }
}

/*
========================================================================================

        create WIN32 LONG_PTR to window, set from lParam,
        so we can have access to class members when handling messages.
        HandleMessageThunk will unpack that pointer, and send it to our
        non-static member function HandleMsg.

========================================================================================
*/
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT Window::HandleMessageSetup(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    if (msg == WM_NCCREATE)
    {

        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);

        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(wnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageThunk));
        return pWnd->HandleMsg(wnd, msg, wParam, lParam);
    }


    return DefWindowProcW(wnd, msg, wParam, lParam);
}

LRESULT Window::HandleMessageThunk(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window* const m_Wnd = reinterpret_cast<Window*>(GetWindowLongPtr(wnd, GWLP_USERDATA));
    return m_Wnd->HandleMsg(wnd, msg, wParam, lParam);
}


LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
   
   
    switch (msg)
    {
    case WM_CLOSE:
        m_shouldClose = true;
        PostQuitMessage(0);
        return 0;
        /*
==================================

        KEYBOARD MESSAGES

==================================
        */

    case WM_LBUTTONDOWN:
        if (IsContextInitialized&&!ImGui::GetIO().WantCaptureMouse)
        {
            std::cout << "Hit";
            DisableCursor(hWnd);
        }
        break;

    case WM_KEYDOWN:
        InputManager::Get().OnKeyPressed(static_cast<unsigned char>(wParam));

        if (wParam == VK_ESCAPE)
        {
            EnableCursor();
        }
        break;

    case WM_KEYUP:
        InputManager::Get().OnKeyReleased(static_cast<unsigned char> (wParam));
        break;

        /*
====================================

          MOUSE MESSAGES

====================================
        */

    case WM_INPUT:
    {
        if (!RawInputListen)
        {
            break;
        }
        UINT size;
        if (GetRawInputData(
            reinterpret_cast<HRAWINPUT>(lParam),
            RID_INPUT,
            nullptr,
            &size,
            sizeof(RAWINPUTHEADER)) == -1)
        {
            break;
        }
        rawBuffer.resize(size);
        if (GetRawInputData(
            reinterpret_cast<HRAWINPUT>(lParam),
            RID_INPUT,
            rawBuffer.data(),
            &size,
            sizeof(RAWINPUTHEADER)) != size)
        {
            break;
        }

        auto& raw_input = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
        if (raw_input.header.dwType == RIM_TYPEMOUSE &&
            (raw_input.data.mouse.lLastX != 0
                ||
                raw_input.data.mouse.lLastY != 0))
        {
            InputManager::Get().HandleMouseRawInput(raw_input.data.mouse.lLastX, raw_input.data.mouse.lLastY);
        }
        break;
    }

    case WM_MOUSEMOVE:
        //InputManager::Get().OnMouseMove(static_cast<uint8_t>(wParam), static_cast<uint32_t>(lParam));
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}


void Window::EnableCursor()
{
    ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    ShowCursor(true);
    ClipCursor(nullptr);
    ReleaseCapture();
    MouseListen = false;
    DisableRawInput();

}

void Window::DisableCursor(const HWND& hWnd)
{
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
    RECT rect;
    GetClientRect(hWnd, &rect);
    rect.left /= 2;
    rect.right /= 2;
    rect.bottom /= 2;
    rect.top /= 2;
    MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
    ClipCursor(&rect);
    ShowCursor(false);
    LockCursorToCenter();
    MouseListen = true;
    EnableRawInput();

}

void Window::LockCursorToCenter()
{
    RECT rect;
    POINT center;
    GetClientRect(m_window, &rect);
    center.x = (rect.left + rect.right) / 2;
    center.y = (rect.left + rect.right) / 2;

   
}

void Window::DisableRawInput()
{
    RawInputListen = false;
}

void Window::EnableRawInput()
{
    RawInputListen = true;
}
