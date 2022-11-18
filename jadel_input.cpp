#include "jadel_input.h"
#include "jadel_math.h"

namespace jadel
{
    const jadel::Window *inputCurrentWindow = NULL;
    int windowWidth = 0;
    int windowHeight = 0;
    int windowHalfW = 0;
    int windowHalfH = 0;

    bool inputKeysPressed[jadel::NUM_KEYS - 1] = {0};
    bool inputKeysReleased[jadel::NUM_KEYS - 1] = {0};
    bool inputKeysTyped[jadel::NUM_KEYS - 1] = {0};

    int inputMouseX = 0;
    int inputMouseY = 0;

    bool inputLButtonClicked = false;
    bool inputRButtonClicked = false;
    bool inputMButtonClicked = false;

    bool inputLButtonReleased = false;
    bool inputRButtonReleased = false;
    bool inputMButtonReleased = false;

    bool inputLButtonHeld = false;
    bool inputRButtonHeld = false;
    bool inputMButtonHeld = false;

    int inputMWheel = 0;

    void inputSetCurrentWindow(const Window *win)
    {
        if (!win)
            return;
        inputCurrentWindow = win;
        inputUpdateCurrentWindow();
    }

    void inputUpdateCurrentWindow()
    {
        windowWidth = inputCurrentWindow->width;
        windowHeight = inputCurrentWindow->height;
        windowHalfW = windowWidth / 2;
        windowHalfH = windowHeight / 2;
    }

    bool inputIsKeyPressed(uint32 key)
    {
        if (key == jadel::KEY_NULL || key >= jadel::NUM_KEYS - 1)
            return false;

        return inputKeysPressed[key - 1];
    }

    bool inputIsKeyReleased(uint32 key)
    {
        if (key == jadel::KEY_NULL || key >= jadel::NUM_KEYS - 1)
            return false;

        return inputKeysReleased[key - 1];
    }

    bool inputIsKeyTyped(uint32 key)
    {
        if (key == jadel::KEY_NULL || key >= jadel::NUM_KEYS - 1)
            return false;

        return inputKeysTyped[key - 1];
    }

    int inputGetMouseX()
    {
        int result = inputMouseX;
        return result;
    }

    int inputGetMouseY()
    {
        int result = windowHeight - inputMouseY;
        return result;
    }

    float inputGetMouseXRelative()
    {
        int mousePosX = inputGetMouseX();
        float result = (float)(mousePosX - windowHalfW) / (float)windowHalfW;
        return result;
    }

    float inputGetMouseYRelative()
    {
        int mousePosY = inputGetMouseY();
        float result = (float)(mousePosY - windowHalfH) / (float)windowHalfH;
        return result;
    }

    jadel::Vec2 inputGetMouseRelative()
    {
        jadel::Vec2 result(inputGetMouseXRelative(), inputGetMouseYRelative());
        return result;
    }

    Point2i inputGetMousePos()
    {
        Point2i result = {inputGetMouseX(), inputGetMouseY()};
        return result;
    }

    bool inputIsMouseLeftClicked()
    {
        bool result = inputLButtonClicked;
        return result;
    }

    bool inputIsMouseLeftHeld()
    {
        bool result = inputLButtonHeld;
        return result;
    }

    bool inputIsMouseLeftReleased()
    {
        bool result = inputLButtonReleased;
        return result;
    }

    bool inputIsMouseRightClicked()
    {
        bool result = inputRButtonClicked;
        return result;
    }

    bool inputIsMouseRightHeld()
    {
        bool result = inputRButtonHeld;
        return result;
    }

    bool inputIsMouseRightReleased()
    {
        bool result = inputRButtonReleased;
        return result;
    }

    bool inputIsMouseMiddleClicked()
    {
        bool result = inputMButtonClicked;
        return result;
    }

    bool inputIsMouseMiddleHeld()
    {
        bool result = inputMButtonHeld;
        return result;
    }

    bool inputIsMouseMiddleReleased()
    {
        bool result = inputMButtonReleased;
        return result;
    }

    void inputUpdate()
    {
        inputMWheel = 0;
        for (int i = 0; i < jadel::NUM_KEYS - 1; ++i)
        {
            inputKeysReleased[i] = false;
            inputKeysTyped[i] = false;
        }

        inputLButtonClicked = false;
        inputRButtonClicked = false;
        inputMButtonClicked = false;

        inputLButtonReleased = false;
        inputRButtonReleased = false;
        inputMButtonReleased = false;

    }
}