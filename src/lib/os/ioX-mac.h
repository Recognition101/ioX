#include <SDL.h>
#include "ApplicationServices/ApplicationServices.h"

namespace ioxOs {
    const int keyboardValues[] = {
        // Modifiers: Cmd, Shift, Caps Lock, Alt, Ctrl
        55, 56, 57, 58, 59,

        // a - z
        0, 11, 8, 2, 14, 3, 5, 4, 34, 38, 40, 37, 46, 45, 31, 35, 12, 15, 1,
        17, 32, 9, 13, 7, 16, 6,

        // Top Row 0 - 9, Minus, Plus
        29, 18, 19, 20, 21, 23, 22, 26, 28, 25, 27, 24,

        // Special Spacing: Space, Return, Tab, Backspace, Delete
        49, 36, 48, 51, 117,
        
        // Special Navigation: Esc, Page Up, Page Down, Home, End, Help
        53, 116, 121, 115, 119, 114,

        // Specials: ~ | : ? " { } < >
        50, 42, 41, 44, 39, 33, 30, 43, 47,
        
        // Navigation: Left, Right, Down, Up
        123, 124, 125, 126,

        // F1 - F12
        122, 120, 99, 118, 96, 97, 98, 100, 101, 109, 103, 111,

        // Number Pad 0 - 9
        82, 83, 84, 85, 86, 87, 88, 89, 91, 92,

        // Number Pad: Plus, Minus, Star, Divide, Period, Equals, Enter, Clear
        69, 78, 67, 75, 65, 81, 76, 71
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
        CGEventRef e1 = CGEventCreateKeyboardEvent(
                NULL,
                (CGKeyCode)vkCode,
                pressed
        );
        CGEventPost(kCGSessionEventTap, e1);
        CFRelease(e1);
        return pressed;
    };

    /**
     * Gets the width (in pixels) of the main screen (the one that moveMouse
     * is relative to).
     * @return the width of the main screen
     */
    int getScreenWidth() {
        return CGDisplayPixelsWide(kCGDirectMainDisplay);
    };

    /**
     * Gets the height (in pixels) of the main screen (the one that moveMouse
     * is relative to).
     * @return the height of the main screen
     */
    int getScreenHeight() {
        return CGDisplayPixelsHigh(kCGDirectMainDisplay);
    };

    /**
     * Sets the state of the mouse.
     * @param x the x-coordinate to move to
     * @param y the y-coordinate to move to
     * @param button the button to press
     * @param pressed the state of the button to set
     */
    bool setMouse(int x, int y, int button, bool pressed) {
        bool isL = button == mouseValues[1];
        bool isR = button == mouseValues[2];

        auto type =
            isL ? (pressed ? kCGEventLeftMouseDown : kCGEventLeftMouseUp) :
            isR ? (pressed ? kCGEventRightMouseDown : kCGEventRightMouseUp) :
            kCGEventMouseMoved;


        CGEventRef ev = CGEventCreateMouseEvent(
            NULL,
            type,
            CGPointMake(x, y),
            kCGMouseButtonLeft // ignored
        );
        CGEventPost (kCGHIDEventTap, ev);
        CFRelease(ev);

        return pressed;
    };
}
