#pragma once

#include "jadel_keys.h"
#include "jadel_defs.h"
#include "jadel_math.h"
#include "jadel_window.h"

namespace jadel
{
    void inputSetCurrentWindow(const Window* win);

    void inputUpdateCurrentWindow();
    
    DECLSPEC bool inputIsKeyPressed(uint32 key);

    DECLSPEC bool inputIsKeyReleased(uint32 key);

    DECLSPEC bool inputIsKeyTyped(uint32 key);

    DECLSPEC void inputSetRelativeMouseMode(bool val);
    
    DECLSPEC void inputSetCursorVisible(bool val);

    DECLSPEC int inputGetMouseX();
    
    DECLSPEC int inputGetMouseY();

    DECLSPEC float inputGetMouseXRelative();
    
    DECLSPEC float inputGetMouseYRelative();

    DECLSPEC jadel::Vec2 inputGetMouseRelative();

    DECLSPEC int inputGetMouseDeltaX();

    DECLSPEC int inputGetMouseDeltaY();

    DECLSPEC float inputGetMouseDeltaXRelative();
    
    DECLSPEC float inputGetMouseDeltaYRelative();

    DECLSPEC jadel::Vec2 inputGetMouseDeltaRelative();

    DECLSPEC Point2i inputGetMouseDelta();

    DECLSPEC bool inputIsMouseLeftClicked();

    DECLSPEC bool inputIsMouseLeftHeld();

    DECLSPEC bool inputIsMouseLeftReleased();

    DECLSPEC bool inputIsMouseRightClicked();

    DECLSPEC bool inputIsMouseRightHeld();

    DECLSPEC bool inputIsMouseRightReleased();

    DECLSPEC bool inputIsMouseMiddleClicked();

    DECLSPEC bool inputIsMouseMiddleHeld();

    DECLSPEC bool inputIsMouseMiddleReleased();

    DECLSPEC int inputGetMouseWheelScrolls();

    DECLSPEC void inputUpdate();


    void inputReset();

    extern bool inputKeysPressed[jadel::NUM_KEYS - 1];

    extern bool inputKeysReleased[jadel::NUM_KEYS - 1];

    extern bool inputKeysTyped[jadel::NUM_KEYS - 1];

    extern int inputMouseX;

    extern int inputMouseY;

    extern int inputMouseDeltaX;
    
    extern int inputMouseDeltaY;

    extern bool inputLButtonClicked;

    extern bool inputRButtonClicked;

    extern bool inputMButtonClicked;

    extern bool inputLButtonReleased;

    extern bool inputRButtonReleased;

    extern bool inputMButtonReleased;

    extern bool inputLButtonHeld;

    extern bool inputRButtonHeld;

    extern bool inputMButtonHeld;

    extern int inputMWheel;
}