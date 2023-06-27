#include "jadel_update.h"
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include "jadel_keys.h"
#include "jadel_graphics.h"
#include "jadel_defs.h"
#include "jadel_file.h"
#include "jadel_math.h"
#include "jadel_png.h"
#include "jadel_carray.h"
#include "jadel_linkedlist.h"
#include "jadel_vector.h"
#include "jadel_memory.h"
#include "jadel_input.h"
#include "jadel_window.h"
#include "jadel_endian.h"
#include "jadel_message.h"
#include "jadel_window.h"

using namespace jadel;
// extern "C" int JadelMain();

HINSTANCE jadel::myHInstance;
TCHAR jadel::className[] = _T("JADELClass");

Window* jadel::windowList[10];
int jadel::numWindows = 0;

namespace jadel
{
    int allocateConsole()
    {
        int result = AllocConsole();
        return result;
    }

    void writeToConsole(const char* text, size_t numChars)
    {
        WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), text, numChars, NULL, NULL);
    }
}

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

LRESULT CALLBACK WndProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam);

bool JadelInit(size_t bytesOfMemoryToReserve)
{
    if (bytesOfMemoryToReserve < KB(1))
    {
        bytesOfMemoryToReserve = KB(1);
    }
    jadel::memoryInit(bytesOfMemoryToReserve);
    determineEndianness();
    graphicsInit();
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
    inputReset();
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    inputUpdate();
}

jadel::Window* findWindow(HWND hWnd)
{
    for (int i = 0; i < jadel::numWindows; ++i)
    {
        if (jadel::windowList[i]->hWnd == hWnd)
        {
            return jadel::windowList[i];
        }
    }
    return NULL;
}

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
        EndPaint(hWnd, &ps);
        return 0;
    case WM_SIZE:
    {
        jadel::Window* win = findWindow(hWnd);
        if (win)
        {
            jadel::windowUpdateSize(win);
            jadel::inputUpdateCurrentWindow();
        }
        return 0;
    }
    case WM_SETFOCUS:
    {
        jadel::Window* win = findWindow(hWnd);
        if (win)
            jadel::inputSetCurrentWindow(win); 
        return 0;
    }
    case WM_KEYDOWN:
        key = convertVKKey(wParam);

        if (key > 0 && key < jadel::NUM_KEYS)
        {
            if (!inputKeysPressed[key - 1])
            {
                inputKeysTyped[key - 1] = true;
            }
            inputKeysPressed[key - 1] = true;
        }
        return 0;
    case WM_KEYUP:
        key = convertVKKey(wParam);
        if (key > 0 && key < jadel::NUM_KEYS)
        {
            inputKeysReleased[key - 1] = true;
            inputKeysPressed[key - 1] = false;
        }
        return 0;
    case WM_MOUSEMOVE:
        inputMouseX = GET_X_LPARAM(lParam);
        inputMouseY = GET_Y_LPARAM(lParam);
        return 0;
    case WM_LBUTTONDOWN:
        inputLButtonClicked = true;
        inputLButtonHeld = true;
        return 0;
    case WM_LBUTTONUP:
        inputLButtonReleased = true;
        inputLButtonHeld = false;
        return 0;
    case WM_RBUTTONDOWN:
        inputRButtonClicked = true;
        inputRButtonHeld = true;
        return 0;
    case WM_RBUTTONUP:
        inputRButtonReleased = true;
        inputRButtonHeld = false;
        return 0;
    case WM_MBUTTONDOWN:
        inputMButtonClicked = true;
        inputMButtonHeld = true;
        return 0;
    case WM_MBUTTONUP:
        inputMButtonReleased = true;
        inputMButtonHeld = false;
        return 0;
    case WM_MOUSEWHEEL:
        inputMWheel = GET_WHEEL_DELTA_WPARAM(wParam) / 120;
        return 0;
    }
    return (DefWindowProc(hWnd, message, wParam, lParam));
}