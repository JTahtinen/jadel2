#pragma once

#ifdef DEBUG
#include "jadel_input.h"
#include "jadel_message.h"
#include "jadel_update.h"

#define JADEL_ASSERT(value, assertionString) assert(value, assertionString)

namespace jadel
{
    inline void assert(bool value, const char *assertionString)
    {
        if (value)
        {
            return;
        }
        inputSetCursorVisible(true);
        inputSetRelativeMouseMode(false);
        JADEL_DEBUGMSG("Assertion failed: %s\n Press ESC to exit program...\n", assertionString);
        while (true)
        {
            JadelUpdate();
            if (inputIsKeyPressed(KEY_ESCAPE))
            {
                exit(0);
            }
        }
    }
}
#else
#define JADEL_ASSERT(value, assertionString)
#endif