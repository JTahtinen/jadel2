#include "window.h"
#include <tchar.h>
#include "winAppTest.h"
//#include "defs.h"
namespace jadel
{
    bool createWindow(Window *target, const char *title, size_t width, size_t height)
    {
        if (!target)
            return false;

        RECT clientRect;
        clientRect.left = 0;
        clientRect.top = 0;
        clientRect.right = width;
        clientRect.bottom = height;

        HWND windowHandle = CreateWindowEx(0,
                                           className,
                                           title,
                                           WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                           CW_USEDEFAULT, CW_USEDEFAULT,
                                           clientRect.right - clientRect.left, clientRect.bottom - clientRect.top,
                                           NULL,
                                           NULL,
                                           myHInstance,
                                           NULL);

        if (!windowHandle)
            return false;
        // if (!createSurface(width, height, &target->winSurface)) return false;

        target->deviceContext = GetDC(windowHandle);
        target->width = width;
        target->height = height;
        target->hWnd = windowHandle;

        target->bitmapInfo.bmiHeader.biSize = sizeof(target->bitmapInfo.bmiHeader);
        target->bitmapInfo.bmiHeader.biWidth = width;
        target->bitmapInfo.bmiHeader.biHeight = height;
        target->bitmapInfo.bmiHeader.biPlanes = 1;
        target->bitmapInfo.bmiHeader.biBitCount = 32;
        target->bitmapInfo.bmiHeader.biCompression = BI_RGB;
        ShowWindow(windowHandle, SW_SHOW);
        return true;
    }

    void updateWindow(Window *win, const Surface *buffer)
    {
        RECT clientRect;
        win->bitmapInfo.bmiHeader.biWidth = buffer->width;
        win->bitmapInfo.bmiHeader.biHeight = buffer->height;

        GetClientRect(win->hWnd, &clientRect);
        StretchDIBits(win->deviceContext,
                      0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top,
                      0, 0, buffer->width, buffer->height,
                      buffer->pixels,
                      &win->bitmapInfo,
                      DIB_RGB_COLORS,
                      SRCCOPY);
    }
}   