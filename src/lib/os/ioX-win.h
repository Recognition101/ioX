#include <SDL.h>
#include <windows.h>

namespace ioxOs {
    const int keyboardValues[] = {
        // Modifiers: Cmd, Shift, Caps Lock, Alt, Ctrl
        VK_LWIN, VK_SHIFT, VK_CAPITAL, VK_MENU, VK_CONTROL,

        // a - z
        0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
        0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E,
        0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55,
        0x56, 0x57, 0x58, 0x59, 0x5A,

        // Top Row 0 - 9, Minus, Plus
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        VK_OEM_MINUS, VK_OEM_PLUS,

        // Special Spacing: Space, Return, Tab, Backspace, Delete
        VK_SPACE, VK_RETURN, VK_TAB, VK_BACK, VK_DELETE,
        
        // Special Navigation: Esc, Page Up, Page Down, Home, End, Help
        VK_ESCAPE, VK_PRIOR, VK_NEXT, VK_HOME, VK_END, VK_HELP,

        // Specials: ~ | : ? " { } < >
        VK_OEM_3, VK_OEM_5, VK_OEM_1, VK_OEM_2, VK_OEM_7,
        VK_OEM_4, VK_OEM_6, VK_OEM_102, VK_OEM_102,
        
        // Navigation: Left, Right, Down, Up
        VK_LEFT, VK_RIGHT, VK_DOWN, VK_UP,

        // F1 - F12
        VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10,
        VK_F11, VK_F12,

        // Number Pad 0 - 9
        VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4,
        VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9,

        // Number Pad: Plus, Minus, Star, Divide, Period, Equals, Enter, Clear
        VK_ADD, VK_SUBTRACT, VK_MULTIPLY, VK_DIVIDE, VK_SEPARATOR,
        VK_OEM_PLUS, VK_RETURN, VK_CLEAR
    };

    const int xboxSeriesValues[] {
        //Axis: Left X, Min, Max
        0, -32767, 32767,

        // Axis: Left Y, Min, Max
        1, -32767, 32767,

        // Axis: Right X, Min, Max
        2, -32767, 32767,

        // Axis: Right Y, Min, Max
        3, -32767, 32767,

        // Axis: Left Trigger, Min, Max
        4, 0, 32767,

        // Axis: Right Trigger, Min, Max
        5, 0, 32767,

        // Buttons: A, B, X, Y, Windows, Xbox, Menu, L3, R3, LB, RB
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,

        // Buttons: Up, Down, Left, Right, Share
        11, 12, 13, 14, 15
    };

    const int mouseValues[] {
        // None, Left, Right
        0, 1, 2
    };

    /**
     * Sets the state of a specific keyboard key.
     * @param vkCode the virtual key code for the keyboard key to press
     * @param pressed true if we are pressing the key, false if releasing
     */
    bool pressKey(int vkCode, bool pressed) {
        INPUT inputs[1] = {};
        ZeroMemory(inputs, sizeof(inputs));
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = vkCode;
        if (!pressed) {
            inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
        }

        UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

        return pressed;
    };

    /**
     * Gets the width (in pixels) of the main screen (the one that moveMouse
     * is relative to).
     * @return the width of the main screen
     */
    int getScreenWidth() {
        return GetSystemMetrics(SM_CXSCREEN);
    };

    /**
     * Gets the height (in pixels) of the main screen (the one that moveMouse
     * is relative to).
     * @return the height of the main screen
     */
    int getScreenHeight() {
        return GetSystemMetrics(SM_CYSCREEN);
    };

    /**
     * Sets the state of the mouse.
     * @param x the x-coordinate to move to
     * @param y the y-coordinate to move to
     * @param button the button to press
     * @param pressed the state of the button to set
     */
    bool setMouse(int x, int y, int button, bool pressed) {
        INPUT input;
        input.type = INPUT_MOUSE;
        input.mi.mouseData = 0;
        input.mi.time = 0;
        input.mi.dx = x * (65536 / GetSystemMetrics(SM_CXSCREEN));
        input.mi.dy = y * (65536 / GetSystemMetrics(SM_CYSCREEN));

        bool isLeft = button == mouseValues[1];
        bool isRight = button == mouseValues[2];

        auto type = isLeft && pressed ? MOUSEEVENTF_LEFTDOWN :
            isLeft && !pressed ? MOUSEEVENTF_LEFTUP :
            isRight && pressed ? MOUSEEVENTF_RIGHTDOWN :
            isRight && !pressed ? MOUSEEVENTF_RIGHTUP :
            MOUSEEVENTF_MOVE;

        input.mi.dwFlags =
            type | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE;
        
        SendInput(1, &input, sizeof(input));

        return pressed;
    };
}
