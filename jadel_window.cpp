#include "jadel_window.h"
#include <tchar.h>
#include "jadel_update.h"
#include "jadel_message.h"
#include "jadel_input.h"

namespace jadel
{
    bool windowCreate(Window *target, const char *title, size_t width, size_t height)
    {
        if (!target)
            return false;

        if (jadel::numWindows < 10)
            jadel::windowList[jadel::numWindows++] = target;
        else 
        {
            jadel::message("[ERROR] Could not create window %s. Too many windows!\n", title);
            return false;
        }   
        RECT clientRect;
        RECT windowRect = {0, 0, width, height};
        /*AdjustWindowRectEx(
            &windowRect,
            0,
            0,
            0);*/ 
        HWND windowHandle = CreateWindowEx(0,
                                           className,
                                           title,
                                           WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                           CW_USEDEFAULT, CW_USEDEFAULT,
                                           windowRect.right - windowRect.left, 
                                           windowRect.bottom - windowRect.top,
                                           NULL,
                                           NULL,
                                           myHInstance,
                                           NULL);

        GetWindowRect(windowHandle, &windowRect);
        GetClientRect(windowHandle, &clientRect);
        POINT ptDiff;
        ptDiff.x = (windowRect.right - windowRect.left) - clientRect.right;
        ptDiff.y = (windowRect.bottom - windowRect.top) - clientRect.bottom;
        MoveWindow(windowHandle, windowRect.left, windowRect.top, width + ptDiff.x, height + ptDiff.y, TRUE);

        if (!windowHandle)
            return false;

        target->width = width;
        target->height = height;
        target->hWnd = windowHandle;

        target->bitmapInfo.bmiHeader.biSize = sizeof(target->bitmapInfo.bmiHeader);
        target->bitmapInfo.bmiHeader.biWidth = width;
        target->bitmapInfo.bmiHeader.biHeight = height;
        target->bitmapInfo.bmiHeader.biPlanes = 1;
        target->bitmapInfo.bmiHeader.biBitCount = 32;
        target->bitmapInfo.bmiHeader.biCompression = BI_RGB;
        jadel::inputSetCurrentWindow(target);
        ShowWindow(windowHandle, SW_SHOW);
        return true;
    }

    void windowUpdate(Window *win, const Surface *buffer)
    {
        RECT clientRect;
        win->bitmapInfo.bmiHeader.biWidth = buffer->width;
        win->bitmapInfo.bmiHeader.biHeight = buffer->height;

        HDC deviceContext = GetDC(win->hWnd);
        GetClientRect(win->hWnd, &clientRect);
        StretchDIBits(deviceContext,
                      0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top,
                      0, 0, buffer->width, buffer->height,
                      buffer->pixels,
                      &win->bitmapInfo,
                      DIB_RGB_COLORS,
                      SRCCOPY);
        ReleaseDC(win->hWnd, deviceContext);
    }

    void windowUpdateSize(Window* win)
    {
        RECT clientRect;
        GetClientRect(win->hWnd, &clientRect);
        win->width = clientRect.right - clientRect.left;
        win->height = clientRect.bottom - clientRect.top;
    }
}   