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
        std::cerr<<"Failed to create window!\n";
    }
    m_window = CreateWindowExW(
        WS_EX_OVERLAPPEDWINDOW | WS_EX_APPWINDOW,
        (LPCWSTR)m_windowClass, L"STAGR",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0,1280 , 720,
        nullptr, nullptr, wcex.hInstance,
        this);

    if (m_window == nullptr)
    {
       std::cerr<< GetLastError()<<"\n";
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
        //==================================
        //       KEYBOARD MESSAGES

    case WM_KEYDOWN:
        InputManager::Get().OnKeyPressed(static_cast<UINT8>(wParam));

        //kbd.OnKeyPressed(static_cast<unsigned char> (wParam));
        break;

    case WM_KEYUP:
        //kbd.OnKeyReleased(static_cast<unsigned char> (wParam));
        break;
        //      END KEYBOARD MESSAGES
        //==================================

    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
