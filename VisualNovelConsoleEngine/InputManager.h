#pragma once
#include <windows.h>

class InputManager
{
	HANDLE hInput;

public:
    enum class Key {
        None,
        Up,
        Down,
        Left,
        Right,
        Enter,
        Escape,
        Space
    };

    InputManager();
    bool HasKey();
    Key GetPressedKey();
    void Flush();
};
