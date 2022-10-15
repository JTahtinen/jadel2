#ifndef WINDOW_H
#define WINDOW_H
#include <windows.h>
#include "graphics.h"
#include "defs.h"

namespace jadel
{
    struct Window
    {
        HWND hWnd;
        HDC deviceContext;
        BITMAPINFO bitmapInfo;
        int width;
        int height;
        // Surface winSurface;
    };

    DLLIMPORT bool createWindow(Window *target, const char *title, size_t width, size_t height);
    DLLIMPORT void updateWindow(Window *win, const Surface *buffer);
}

#endif