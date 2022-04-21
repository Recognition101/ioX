#include <node.h>

#ifdef _WIN32
#include "os/ioX-win.h"

#elif __APPLE__
#include "os/ioX-mac.h"
#endif

namespace iox {

using v8::Function;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Context;
using v8::Object;
using v8::String;
using v8::Number;
using v8::Value;
using v8::Null;
using v8::ReadOnly;

bool hasStarted = false;

const int keyboardCount = 97;
const char* keyboardNames[] = {
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
};

const int xboxSeriesCount = 34;
const char* xboxSeriesNames[] = {
    "leftStickX", "leftStickXMin", "leftStickXMax",
    "leftStickY", "leftStickYMin", "leftStickYMax",
    "rightStickX", "rightStickXMin", "rightStickXMax",
    "rightStickY", "rightStickYMin", "rightStickYMax",
    "leftTrigger", "leftTriggerMin", "leftTriggerMax",
    "rightTrigger", "rightTriggerMin", "rightTriggerMax",
    "a", "b", "x", "y", "multitask", "xbox", "menu", "l3", "r3", "lb", "rb",
    "up", "down", "left", "right", "share"
};

const int mouseCount = 3;
const char* mouseNames[] = { "none", "left", "right" };

const char* emptyNames[] = { "" };
const int emptyValues[] = { 0 };

// TODO: getEvents should own event loop & sleep w/SDL_Delay(15), not setInteval
//  * Callback to getEvents could return SLEEP, NEXT_FRAME, or EXIT
//      * SLEEP - don't call again until a non-empty event
//      * NEXT_FRAME - call back with next frame, even if nothing happened
//      * EXIT - exit the event loop


void getCodes(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> obj = Object::New(isolate);

    Local<String> strKeyboard = String::NewFromUtf8(isolate, "keyboard")
        .ToLocalChecked();
    Local<String> strXboxSeries = String::NewFromUtf8(isolate, "xboxSeries")
        .ToLocalChecked();
    Local<String> strMouse = String::NewFromUtf8(isolate, "mouse")
        .ToLocalChecked();

    bool isKeyboard = args[0]->StrictEquals(strKeyboard);
    bool isXboxSeries = args[0]->StrictEquals(strXboxSeries);
    bool isMouse = args[0]->StrictEquals(strMouse);

    int count =
        isKeyboard ? iox::keyboardCount :
        isXboxSeries ? iox::xboxSeriesCount :
        isMouse ? iox::mouseCount :
        0;

    const char** names =
        isKeyboard ? iox::keyboardNames :
        isXboxSeries ? iox::xboxSeriesNames :
        isMouse ? iox::mouseNames :
        iox::emptyNames;

    const int* values =
        isKeyboard ? ioxOs::keyboardValues :
        isXboxSeries ? ioxOs::xboxSeriesValues :
        isMouse ? ioxOs::mouseValues :
        iox::emptyValues;

    for(int i = 0; i < count; i += 1) {
        obj->Set(
            context,
            String::NewFromUtf8(isolate, names[i]).ToLocalChecked(),
            Number::New(isolate, values[i])
        ).FromJust();
    }

    args.GetReturnValue().Set(obj);
}

void getEvents(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    Local<Function> cb = Local<Function>::Cast(args[0]);

    auto setString = [&](
        Local<Object>* result,
        const char* key,
        const char* value
    ) {
        (*result)->Set(
            context,
            String::NewFromUtf8(isolate, key).ToLocalChecked(),
            String::NewFromUtf8(isolate, value).ToLocalChecked()
        ).FromJust();
    };

    auto setNumber = [&](Local<Object>* result, const char* key, int value) {
        (*result)->Set(
            context,
            String::NewFromUtf8(isolate, key).ToLocalChecked(),
            Number::New(isolate, value)
        ).FromJust();
    };

    auto setCall = [&](Local<Object>* result) {
        const unsigned argc = 1;
        Local<Value> argv[argc] = { (*result) };
        cb->Call(context, Null(isolate), argc, argv).ToLocalChecked();
    };

    if (!iox::hasStarted) {
        SDL_SetHint(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, "0");
        SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_JOY_CONS, "1");
        SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS4_RUMBLE, "1");
        SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5_RUMBLE, "1");
        SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            Local<Object> result = Object::New(isolate);
            setString(&result, "type", "error");
            setString(&result, "message", "Could not start SDL.");
            setCall(&result);
            return;
        }
    }

    iox::hasStarted = true;

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_CONTROLLERDEVICEADDED) {
            auto gc = SDL_GameControllerOpen(event.cdevice.which);

            Local<Object> result = Object::New(isolate);
            setString(&result, "type", "gamepadAdd");
            setString(&result, "name", SDL_GameControllerName(gc));
            setNumber(&result, "gamepadId", event.cdevice.which);
            setNumber(&result, "vendorId", SDL_GameControllerGetVendor(gc));
            setNumber(&result, "productId", SDL_GameControllerGetProduct(gc));
            setCall(&result);

        } else if (event.type == SDL_CONTROLLERDEVICEREMOVED) {
            Local<Object> result = Object::New(isolate);
            setString(&result, "type", "gamepadRemove");
            setNumber(&result, "gamepadId", event.cdevice.which);
            setCall(&result);

        } else if (event.type == SDL_CONTROLLERAXISMOTION) {
            Local<Object> result = Object::New(isolate);
            setString(&result, "type", "axisChange");
            setNumber(&result, "gamepadId", event.cdevice.which);
            setNumber(&result, "id", event.jaxis.axis);
            setNumber(&result, "value", event.jaxis.value);
            setNumber(&result, "time", event.jaxis.timestamp);
            setCall(&result);

        } else if (event.type == SDL_CONTROLLERBUTTONDOWN) {
            Local<Object> result = Object::New(isolate);
            setString(&result, "type", "buttonChange");
            setNumber(&result, "gamepadId", event.cdevice.which);
            setNumber(&result, "id", event.cbutton.button);
            setNumber(&result, "value", true);
            setNumber(&result, "time", event.cbutton.timestamp);
            setCall(&result);

        } else if (event.type == SDL_CONTROLLERBUTTONUP) {
            Local<Object> result = Object::New(isolate);
            setString(&result, "type", "buttonChange");
            setNumber(&result, "gamepadId", event.cdevice.which);
            setNumber(&result, "id", event.cbutton.button);
            setNumber(&result, "value", false);
            setNumber(&result, "time", event.cbutton.timestamp);
            setCall(&result);

        }

        // TODO: Sensors, Touchpad
    }
}

void pressKey(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();

    int vkCode = args[0]->IntegerValue(context).ToChecked();
    bool pressed = args[1]->BooleanValue(isolate);

    ioxOs::pressKey(vkCode, pressed);
}

void setMouse(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();

    int x = args[0]->IntegerValue(context).ToChecked();
    int y = args[1]->IntegerValue(context).ToChecked();
    int type = args[2]->IntegerValue(context).ToChecked();
    bool pressed = args[3]->BooleanValue(isolate);

    ioxOs::setMouse(x, y, type, pressed);
}

void getScreenSize(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> result = Object::New(isolate);


    result->Set(
        context,
        String::NewFromUtf8(isolate, "width").ToLocalChecked(),
        Number::New(isolate, ioxOs::getScreenWidth())
    ).FromJust();

    result->Set(
        context,
        String::NewFromUtf8(isolate, "height").ToLocalChecked(),
        Number::New(isolate, ioxOs::getScreenHeight())
    ).FromJust();

    args.GetReturnValue().Set(result);
}

void Initialize(Local<Object> exports) {
    NODE_SET_METHOD(exports, "getCodes", getCodes);
    NODE_SET_METHOD(exports, "getEvents", getEvents);
    NODE_SET_METHOD(exports, "pressKey", pressKey);
    NODE_SET_METHOD(exports, "setMouse", setMouse);
    NODE_SET_METHOD(exports, "getScreenSize", getScreenSize);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
}
