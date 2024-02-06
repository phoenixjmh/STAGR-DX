#include "InputManager.h"
#include "Renderer.h"
#include "windowsx.h"



using KB_EVENT_TYPE = InputManager::KeyboardEvent::Type;


void InputManager::HandleInput(float time_step)
{
    //handle this better it's super annoying

    Camera* s_Camera = &Renderer::Get().GetCamera();

    if (KeyIsPressed(VK_RIGHT))
    {
        s_Camera->MoveRight(time_step);
    }
    if (KeyIsPressed(VK_LEFT))
    {
        s_Camera->MoveLeft(time_step);
    }
    if (KeyIsPressed(VK_UP))
    {
        s_Camera->MoveForward(time_step);
    }
    if (KeyIsPressed(VK_DOWN))
    {
        s_Camera->MoveBackward(time_step);
    }

    

    if (MouseMoved)
    {
        //Log("Move registered input");
        s_Camera->MouseMoveFirstPerson(MouseXDelta, MouseYDelta,time_step);
        MouseMoved = false;
    }


}






bool InputManager::KeyIsPressed(unsigned char keycode) const noexcept
{
    return m_KeyStates[keycode];
}

InputManager::KeyboardEvent InputManager::ReadKey() noexcept
{
    if (m_KeyBuffer.size() > 0)
    {
        InputManager::KeyboardEvent e = m_KeyBuffer.front();
        m_KeyBuffer.pop();
        return e;
    }
    else
    {
        return KeyboardEvent();

    }

}
bool InputManager::KeyIsEmpty() const noexcept
{
    return m_KeyBuffer.empty();
}

void InputManager::FlushKey() noexcept
{
    m_KeyBuffer = std::queue<KeyboardEvent>();
}

void InputManager::Flush() noexcept
{
    FlushKey();
}

/*
   ====================================================

               EXPOSED TO WINDOW CLASS

   ====================================================
   */
void InputManager::OnKeyPressed(unsigned char KEYCODE) noexcept
{
    m_KeyStates[KEYCODE] = true;
    m_KeyBuffer.push(InputManager::KeyboardEvent(KB_EVENT_TYPE::PRESS, KEYCODE));
    TrimBuffer(m_KeyBuffer);
}

void InputManager::OnKeyReleased(unsigned char KEYCODE) noexcept
{
    m_KeyStates[KEYCODE] = false;
    m_KeyBuffer.push(InputManager::KeyboardEvent(KB_EVENT_TYPE::RELEASE, KEYCODE));
    TrimBuffer(m_KeyBuffer);
}

void InputManager::ClearState() noexcept
{
    m_KeyStates.reset();
}



//If buffer size exceeds the declared size of the bitset, we 
//remove the event next in line until the queue is the correct size
template<typename T>
inline void InputManager::TrimBuffer(std::queue<T>& buffer) noexcept
{
    while (buffer.size() > s_BufferSize)
    {
        buffer.pop();
    }
}


/*
   ====================================================

                        MOUSE

   ====================================================
   */

//void InputManager::OnMouseMove(uint8_t wParam, uint32_t lParam)
//{
//    if (!MouseEnabled)
//        return;
//
//    MouseXPos = GET_X_LPARAM(lParam);
//    MouseYPos = GET_Y_LPARAM(lParam);
//    MouseMoved = true;
//    
//}

void InputManager::HandleMouseRawInput(int x_delta,int y_delta)
{
    //if (-1 < x_delta < 1 && -1 < y_delta < 1)
    //{
    //    //this rejects all values where
    //    // x and y and negative at the same time
    //    //return;
    //}
    //    Log(x_delta, y_delta, "Not Registered");
    
        MouseXDelta = x_delta;
        MouseYDelta = y_delta;
        MouseMoved = true;
    
}