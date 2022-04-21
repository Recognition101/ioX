declare module "*ioX" {
    // Get Codes Function
    type CodesKeyboardNames = [
        "cmd", "shift", "capsLock", "alt", "ctrl",

        "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n",
        "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",

        "topRow0", "topRow1", "topRow2", "topRow3", "topRow4", "topRow5",
        "topRow6", "topRow7", "topRow8", "topRow9", "topRowMinus", "topRowPlus",

        "space", "ret", "tab", "backspace", "del",
        
        "escape", "pageUp", "pageDown", "home", "end", "help",

        "tilde", "pipe", "colon", "questionMark", "quote",
        "braceLeft", "braceRight", "lessThan", "greaterThan",
        
        "left", "right", "down", "up",

        "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11",
        "f12",

        "numberPad0", "numberPad1", "numberPad2", "numberPad3", "numberPad4",
        "numberPad5", "numberPad6", "numberPad7", "numberPad8", "numberPad9",
        "numberPadPlus", "numberPadMinus", "numberPadStar", "numberPadDivide",
        "numberPadPeriod", "numberPadEquals", "numberPadEnter", "numberPadClear"
    ];

    type CodesXboxSeriesNames = [
        "leftStickX", "leftStickXMin", "leftStickXMax",
        "leftStickY", "leftStickYMin", "leftStickYMax",
        "rightStickX", "rightStickXMin", "rightStickXMax",
        "rightStickY", "rightStickYMin", "rightStickYMax",
        "leftTrigger", "leftTriggerMin", "leftTriggerMax",
        "rightTrigger", "rightTriggerMin", "rightTriggerMax",
        "a", "b", "x", "y", "multitask", "xbox", "menu", "l3", "r3", "lb", "rb",
        "up", "down", "left", "right", "share"
    ];

    type CodesMouseNames = [ "none", "left", "right" ];

    type CodesKeyboard = { [key in CodesKeyboardNames[number]]: number };
    type CodesXboxSeries = { [key in CodesXboxSeriesNames[number]]: number };
    type CodesMouse = { [key in CodesMouseNames[number]]: number };

    export function getCodes(type: "keyboard"): CodesKeyboard;
    export function getCodes(type: "xboxSeries"): CodesXboxSeries;
    export function getCodes(type: "mouse"): CodesMouse;


    // Get Events Function
    type EventError = { type: "error"; message: string; };
    type EventGamepadAdd = {
        type: "gamepadAdd";
        name: string;
        gamepadId: number;
        vendorId: number;
        productId: number;
    };
    type EventGamepadRemove = {
        type: "gamepadRemove";
        gamepadId: number;
    };
    type EventAxisChange = {
        type: "axisChange";
        gamepadId: number;
        id: number;
        value: number;
        time: number;
    }
    type EventButtonChange = {
        type: "buttonChange";
        gamepadId: number;
        id: number;
        value: boolean;
        time: number;
    }

    type Event =
        | EventError
        | EventGamepadAdd
        | EventGamepadRemove
        | EventAxisChange
        | EventButtonChange;

    export function getEvents(callback: ((event: Event) => void)): void;

    // Press Key
    export function pressKey(code: number, pressed: boolean): void;

    // Set Mouse
    export function setMouse(
        x: number,
        y: number,
        code: number,
        pressed: boolean
    ): void;

    // Get Screen Size
    type Size = {
        width: number;
        height: number;
    };
    export function getScreenSize(): Size;

}
