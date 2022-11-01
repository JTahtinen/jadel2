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

    DECLSPEC void inputUpdate();

    DECLSPEC extern bool inputKeysPressed[jadel::NUM_KEYS - 1];

    DECLSPEC extern bool inputKeysReleased[jadel::NUM_KEYS - 1];

    DECLSPEC extern bool inputKeysTyped[jadel::NUM_KEYS - 1];

    DECLSPEC extern int inputMouseX;

    DECLSPEC extern int inputMouseY;

    DECLSPEC extern bool inputLButtonDown;

    DECLSPEC extern bool inputRButtonDown;

    DECLSPEC extern bool inputMButtonDown;

    DECLSPEC extern int inputMWheel;
}