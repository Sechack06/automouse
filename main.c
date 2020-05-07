#define _CRT_OBSOLETE_NO_WARNINGS
#include <Windows.h>
#include <stdbool.h>
#include <stdlib.h>

WNDCLASSA WndClass;
MSG msg;
HINSTANCE g_hInst;
POINT ptMouse;
bool automousetoogle = false;
HANDLE autoclickthread;
#define STARTBTN 1
#define STOPBTN 2

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

DWORD WINAPI autoclick(void* lpVoid)
{
    while(true)
    {
        if (automousetoogle)
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 100, 100, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 100, 100, 0, 0);
            _sleep(50);
        }
    }
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    WndClass.lpszClassName = "mainwindow";
    WndClass.hInstance = hInstance;
    WndClass.lpfnWndProc = WinProc;
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.lpszMenuName = NULL;

    RegisterClassA(&WndClass);

    CreateWindowA("mainwindow", "auto mouse", WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_BORDER, 300, 200, 400, 100, NULL, NULL, g_hInst, NULL);

    while (GetMessageA(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

	return 0;
}
LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (Msg == WM_HOTKEY)
    {
        if (wParam == 1)
        {
            automousetoogle = true;
        }
        else if (wParam == 0)
        {
            automousetoogle = false;
        }
    }
    switch (Msg)
    {
    case WM_CLOSE:
        CloseHandle(autoclickthread);
        PostQuitMessage(0);
        break;
    case WM_CREATE:
        autoclickthread = CreateThread(NULL, 0, autoclick, &automousetoogle, 0, NULL);
        RegisterHotKey(hWnd, 1, 0, VK_F5);
        RegisterHotKey(hWnd, 0, 0, VK_F6);
        CreateWindowA("button", "시작(F5)", WS_VISIBLE | WS_CHILD, 70, 15, 100, 30, hWnd, (HMENU)STARTBTN, g_hInst, NULL);
        CreateWindowA("button", "중지(F6)", WS_VISIBLE | WS_CHILD, 200, 15, 100, 30, hWnd, (HMENU)STOPBTN, g_hInst, NULL);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case STARTBTN:
            automousetoogle = true;
            break;
        case STOPBTN:
            automousetoogle = false;
            break;
        default:
            break;
        }
    default:
        break;
    }
    return DefWindowProcA(hWnd, Msg, wParam, lParam);
}