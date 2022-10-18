#pragma once

#include "jadel_keys.h"
#include "jadel_defs.h"

namespace jadel
{
    DECLSPEC bool inputIsKeyPressed(uint32 key);

    DECLSPEC bool inputIsKeyReleased(uint32 key);

    DECLSPEC bool inputIsKeyTyped(uint32 key);

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