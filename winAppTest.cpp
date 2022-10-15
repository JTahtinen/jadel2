#include "winAppTest.h"
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include "keys.h"
#include "graphics.h"
#include "defs.h"
#include "file.h"
#include "math.h"
#include "png.h"
#include "carray.h"
#include "linkedlist.h"
#include "vector.h"
#include "memory.h"
#include "input.h"
#include "window.h"

using namespace jadel;
//extern "C" int JadelMain();

Endianness jadel::endianness;
HINSTANCE jadel::myHInstance;
TCHAR jadel::className[] = _T("JADELClass");

static char keyToAscii(uint32 key)
{
    if (key >= KEY_A && key <= KEY_Z)
    {
        return 0x41 + (key - KEY_A);
    }
    switch (key)
    {
    case KEY_SPACE:
        return ' ';
    }
    return 0;
}

static void determineEndianness()
{
    uint32 x = 0x76543210;
    char *c = (char *)&x;

    if (*c == 0x10)
    {
        endianness = LITTLE_ENDIAN;
    }
    else
    {
        endianness = BIG_ENDIAN;
    }
}

LRESULT CALLBACK WndProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam);

bool JadelInit()
{
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = myHInstance;
    wc.hIcon = LoadIcon(wc.hInstance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = className;
    wc.hIconSm = LoadIcon(wc.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, (LPCSTR) "Window class registering failed", (LPCSTR) "Error!", 0);
        return false;
    }
    return true;
}

void JadelUpdate()
{
    MSG msg;
    updateInput();
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}



/*int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{

    initMemory(MB(1));
    memoryPrintDebugData();

    Surface testSurface;
    if (loadBMP("dude1.bmp", &testSurface))
    {
        printf("BMP load success!\nWidth: %d, Height: %d\n", testSurface.width, testSurface.height);
    }

    /*   if (loadPNG("dude2.png", &testSurface))
       {
           printf("PNG load success!\nwidth: %d, height: %d\n", testSurface.width, testSurface.height);

       }
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(wc.hInstance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = className;
    wc.hIconSm = LoadIcon(wc.hInstance, IDI_APPLICATION);

    float colorR = 0;
    float colorG = 0;

    determineEndianness();
    initGraphics();

    Surface background;
    if (!createSurface(400, 300, &background))
        return 0;

    Pixel screenPix = {.content = 0xff000000};

    //        AdjustWindowRectEx(&clientRect, WS_OVERLAPPEDWINDOW, 0, 0);
    //        printf("w: %d, h: %d\n", clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, (LPCSTR) "Window class registering failed", (LPCSTR) "Error!", 0);
        return 0;
    }

    Window win;
    Window win2;

    if (!createWindow(&win, "My beautiful app", 800, 600))
    {
        MessageBox(NULL, _T("Could not create window"), _T("Error!"), 0);
        return 0;
    }
    if (!createWindow(&win2, "My other beautiful app", 400, 300))
    {
        MessageBox(NULL, _T("Could not create window"), _T("Error!"), 0);
        return 0;
    }
    pushTargetSurface(&background);
    jadel::fill(0xffff0000);
    // ShowWindow(hwnd, nCmdShow);
    MSG msg;

    bool running = true;
    unsigned char sr = 0;
    unsigned char sg = 1;

    int xPos = 200;
    int yPos = 100;
    Color newColor = {.a = 1, .r = 0, .g = 1, .b = 0};
    float cAlpha = 0.0f;
    float speed = 1.0f;

    JadelMain();
    while (running)
    {
        updateInput();
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                running = false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (lButtonDown)
        {
            colorR = (float)jadel::mouseX / 800.0f;
            colorG = (float)jadel::mouseY / 600.0f;

            if (colorR < 0)
                colorR = 0;
            if (colorR > 1.0f)
                colorR = 1.0f;

            if (colorG < 0)
                colorG = 0;
            if (colorG > 1.0f)
                colorG = 1.0f;

            sr = colorR * (float)0xff;
            sg = colorG * (float)0xff;
            printf("%f: %d, %f: %d\n", colorR, sr, colorG, sg);
        }
        fill(0xffff0000);
        if (isKeyPressed(KEY_SHIFT))
        {
            speed = 2.5f;
        }
        else if (isKeyReleased(KEY_SHIFT))
        {
            speed -= 1.5f;
        }

        if (isKeyPressed(KEY_DOWN))
            yPos -= roundToInt(speed);
        if (isKeyPressed(KEY_UP))
            yPos += roundToInt(speed);
        if (isKeyPressed(KEY_RIGHT))
            xPos += roundToInt(speed);
        if (isKeyPressed(KEY_LEFT))
            xPos -= roundToInt(speed);
        if (isKeyPressed(KEY_W))
            cAlpha += 0.01f;
        if (isKeyPressed(KEY_S))
            cAlpha -= 0.01f;
        if (mWheel)
            cAlpha += (float)mWheel * 0.03f;
        cAlpha = clampf(cAlpha, 0, 1.0f);
        newColor.a = cAlpha;
        blit(&testSurface, 200, 0);
        drawRect(100 + xPos, 200 + xPos, 100 + yPos, 200 + yPos, newColor);

        if (isKeyPressed(KEY_ESCAPE))
        {
            running = false;
        }

        updateWindow(&win, &background);
        updateWindow(&win2, &background);
    }
    return msg.wParam;*
}*/

LRESULT CALLBACK WndProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Hello, world!");
    char typed = 0;
    uint32 key = KEY_NULL;
    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // GetClientRect(hWnd, &clientRect);
        // updateWindow(hdc, &clientRect);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_KEYDOWN:
        key = convertVKKey(wParam);

        if (key > 0 && key < jadel::NUM_KEYS)
        {
            if (!keysPressed[key - 1])
            {
                keysTyped[key - 1] = true;
            }
            keysPressed[key - 1] = true;
        }
        return 0;
    case WM_KEYUP:
        key = convertVKKey(wParam);
        if (key > 0 && key < jadel::NUM_KEYS)
        {
            keysReleased[key - 1] = true;
            keysPressed[key - 1] = false;
        }
    case WM_MOUSEMOVE:
        mouseX = GET_X_LPARAM(lParam);
        mouseY = GET_Y_LPARAM(lParam);
        return 0;
    case WM_LBUTTONDOWN:
        lButtonDown = true;
        return 0;
    case WM_LBUTTONUP:
        lButtonDown = false;
        return 0;
    case WM_RBUTTONDOWN:
        rButtonDown = true;
        return 0;
    case WM_RBUTTONUP:
        rButtonDown = false;
        return 0;
    case WM_MBUTTONDOWN:
        mButtonDown = true;
        return 0;
    case WM_MBUTTONUP:
        mButtonDown = false;
        return 0;
    case WM_MOUSEWHEEL:
        mWheel = GET_WHEEL_DELTA_WPARAM(wParam) / 120;
        return 0;
    }
    return (DefWindowProc(hWnd, message, wParam, lParam));
}