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
    
    DECLSPEC int inputGetMouseX();
    
    DECLSPEC int inputGetMouseY();

    DECLSPEC float inputGetMouseXRelative();
    
    DECLSPEC float inputGetMouseYRelative();

    DECLSPEC jadel::Vec2 inputGetMouseRelative();

    DECLSPEC Point2i inputGetMousePos();

    DECLSPEC bool inputIsMouseLeftClicked();

    DECLSPEC bool inputIsMouseLeftHeld();

    DECLSPEC bool inputIsMouseRightClicked();

    DECLSPEC bool inputIsMouseRightHeld();

    DECLSPEC bool inputIsMouseMiddleClicked();

    DECLSPEC bool inputIsMouseMiddleHeld();

    DECLSPEC void inputUpdate();

    extern bool inputKeysPressed[jadel::NUM_KEYS - 1];

    extern bool inputKeysReleased[jadel::NUM_KEYS - 1];

    extern bool inputKeysTyped[jadel::NUM_KEYS - 1];

    extern int inputMouseX;

    extern int inputMouseY;

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