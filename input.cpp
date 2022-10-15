#include "input.h"
namespace jadel
{
    bool keysPressed[jadel::NUM_KEYS - 1] = {0};
    bool keysReleased[jadel::NUM_KEYS - 1] = {0};
    bool keysTyped[jadel::NUM_KEYS - 1] = {0};

    int mouseX = 0;
    int mouseY = 0;
    bool lButtonDown = false;
    bool rButtonDown = false;
    bool mButtonDown = false;
    int mWheel = 0;

    bool isKeyPressed(uint32 key)
    {
        if (key == jadel::KEY_NULL || key >= jadel::NUM_KEYS - 1)
            return false;

        return keysPressed[key - 1];
    }

    bool isKeyReleased(uint32 key)
    {
        if (key == jadel::KEY_NULL || key >= jadel::NUM_KEYS - 1)
            return false;

        return keysReleased[key - 1];
    }

    bool isKeyTyped(uint32 key)
    {
        if (key == jadel::KEY_NULL || key >= jadel::NUM_KEYS - 1)
            return false;

        return keysTyped[key - 1];
    }

    void updateInput()
    {
        mWheel = 0;
        for (int i = 0; i < jadel::NUM_KEYS - 1; ++i)
        {
            keysReleased[i] = false;
            keysTyped[i] = false;
        }
    }
}