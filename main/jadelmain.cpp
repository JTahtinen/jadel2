#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <string.h> 
//#include "winapptest.h"

extern "C" int JadelMain();

namespace jadel
{
    extern __declspec(dllimport) HINSTANCE myHInstance;
    extern __declspec(dllimport) TCHAR className[];
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    jadel::myHInstance = hInstance;
    return JadelMain();
}