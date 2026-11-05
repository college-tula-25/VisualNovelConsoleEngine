#include "InputManager.h"

InputManager::InputManager()
{
    hInput = GetStdHandle(STD_INPUT_HANDLE);

    DWORD mode;
    GetConsoleMode(hInput, &mode);
    SetConsoleMode(hInput, (mode & ~ENABLE_QUICK_EDIT_MODE) | ENABLE_WINDOW_INPUT);
}

bool InputManager::HasKey()
{
    DWORD events{};
    GetNumberOfConsoleInputEvents(hInput, &events);
    return events > 0;
}

InputManager::Key InputManager::GetPressedKey()
{
    DWORD read;
    INPUT_RECORD record;

    if (!HasKey()) return Key::None;

    if (ReadConsoleInputA(hInput, &record, 1, &read))
    {
        if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown)
        {
            WORD vCode = record.Event.KeyEvent.wVirtualKeyCode;

            switch (vCode)
            {
            case VK_UP:     return Key::Up;
            case VK_DOWN:   return Key::Down;
            case VK_LEFT:   return Key::Left;
            case VK_RIGHT:  return Key::Right;
            case VK_RETURN: return Key::Enter;
            case VK_ESCAPE: return Key::Escape;
            case VK_SPACE:  return Key::Space;
            default:        return Key::None;
            }
        }
    }
    return Key::None;
}

void InputManager::Flush()
{
    FlushConsoleInputBuffer(hInput);
}
