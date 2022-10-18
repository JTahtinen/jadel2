#include "jadel_input.h"
namespace jadel
{
    bool inputKeysPressed[jadel::NUM_KEYS - 1] = {0};
    bool inputKeysReleased[jadel::NUM_KEYS - 1] = {0};
    bool inputKeysTyped[jadel::NUM_KEYS - 1] = {0};

    int inputMouseX = 0;
    int inputMouseY = 0;
    bool inputLButtonDown = false;
    bool inputRButtonDown = false;
    bool inputMButtonDown = false;
    int inputMWheel = 0;

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

    void inputUpdate()
    {
        inputMWheel = 0;
        for (int i = 0; i < jadel::NUM_KEYS - 1; ++i)
        {
            inputKeysReleased[i] = false;
            inputKeysTyped[i] = false;
        }
    }
}