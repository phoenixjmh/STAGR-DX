#pragma once
#include "WinInclude.h"
#include <queue>
#include <bitset>
class InputManager
{
    friend class Window;
public:
    class KeyboardEvent
    {
    public:

        enum class Type
        {
            PRESS,
            RELEASE,
            INVALID
        };
    private:

        Type m_type;

        unsigned char m_code;

    public:
        KeyboardEvent() :
            m_type(Type::INVALID),
            m_code(0u)
        {
        }
        KeyboardEvent(Type type, unsigned char KEY)
            :m_type(type), m_code(KEY)
        {
        }
        bool IsPress() { return m_type == Type::PRESS; }

        bool IsRelease() { return m_type == Type::RELEASE; }

        bool IsValid() { return m_type == Type::INVALID; }

        unsigned char GetCode() { return m_code; }

    };


public:


    InputManager(const InputManager&) = delete;
    InputManager& operator = (const InputManager&) = delete;

    inline static InputManager& Get()
    {
        static InputManager instance;
        return instance;
    }
    void HandleInput(float time_step);

    InputManager() = default;

    bool MouseEnabled = false;

    bool KeyIsPressed(unsigned char keycode) const noexcept;

    KeyboardEvent ReadKey() noexcept;

    bool KeyIsEmpty() const noexcept;

    void FlushKey() noexcept;

    void Flush() noexcept;

private:
    /*
    ====================================================

                EXPOSED TO WINDOW CLASS

    ====================================================
    */
    void OnKeyPressed(unsigned char keycode) noexcept;

    void OnKeyReleased(unsigned char keybode) noexcept;

    void ClearState() noexcept;


    template<typename T>
    static void TrimBuffer(std::queue<T>& buffer) noexcept;

private:
    static constexpr unsigned int s_Keys = 256;

    static constexpr unsigned int s_BufferSize = 16u;

    std::bitset<s_Keys> m_KeyStates;

    std::queue<KeyboardEvent> m_KeyBuffer;



    /*
   ====================================================

                        MOUSE

   ====================================================
   */

public:

    void OnMouseMove(uint8_t wParam, uint32_t lParam);

    void HandleMouseRawInput(int x_delta, int y_delta);


private:
    bool MouseMoved=false;

    int MouseXDelta, MouseYDelta;
};


