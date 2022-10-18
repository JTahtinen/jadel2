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
        // Surface winSurface;
    };

    DECLSPEC bool windowCreate(Window *target, const char *title, size_t width, size_t height);
    DECLSPEC void windowUpdate(Window *win, const Surface *buffer);
}