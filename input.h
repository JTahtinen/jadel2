#ifndef INPUT_H
#define INPUT_H
#include "keys.h"
#include "defs.h"

namespace jadel
{
    DLLIMPORT bool isKeyPressed(uint32 key);

    DLLIMPORT bool isKeyReleased(uint32 key);

    DLLIMPORT bool isKeyTyped(uint32 key);

    DLLIMPORT void updateInput();

    extern DLLIMPORT bool keysPressed[jadel::NUM_KEYS - 1];

    extern DLLIMPORT bool keysReleased[jadel::NUM_KEYS - 1];

    extern DLLIMPORT bool keysTyped[jadel::NUM_KEYS - 1];

    extern DLLIMPORT int mouseX;

    extern DLLIMPORT int mouseY;

    extern DLLIMPORT bool lButtonDown;

    extern DLLIMPORT bool rButtonDown;

    extern DLLIMPORT bool mButtonDown;

    extern DLLIMPORT int mWheel;
}
#endif