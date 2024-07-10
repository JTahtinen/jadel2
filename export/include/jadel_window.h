#pragma once

#include <windows.h>
#include "jadel_graphics.h"
#include "jadel_defs.h"

namespace jadel
{
    struct Window
    {
        HWND hWnd;
        BITMAPINFO bitmapInfo;
        int width;
        int height;
        float aspect;
        // Surface winSurface;
    };

    DECLSPEC bool windowCreate(Window *target, const char *title, int width, int height);
    DECLSPEC void windowUpdate(Window *target, const Surface *buffer);
    DECLSPEC bool windowSetSize(Window *target, int width, int height);
    void windowUpdateSize(Window *target);
    void windowMaintainAspect(Window *target);
}