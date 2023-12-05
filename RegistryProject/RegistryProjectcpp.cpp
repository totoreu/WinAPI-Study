#include <Windows.h>
#include <iostream>
#include <string>
#include "Reg.h"
#include "Utility.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("RegistryProject");

int APIENTRY WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdParam,
    int nCmdShow)
{
    HWND hWnd;
    MSG Message;
    WNDCLASS WndClass;
    g_hInst = hInstance;

    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hInstance = hInstance;
    WndClass.lpfnWndProc = WndProc;
    WndClass.lpszClassName = lpszClass;
    WndClass.lpszMenuName = NULL;
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);

    hWnd = CreateWindow(
        lpszClass,
        lpszClass,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        (HMENU)NULL,
        hInstance,
        NULL
    );

    ShowWindow(hWnd, nCmdShow);

    while (GetMessage(&Message, NULL, 0, 0))
    {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return (int)Message.wParam;
}

constexpr const LPCTSTR KEY = TEXT("Software\\HS\\RegTest\\");

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    RECT rt;
    static HWND hEdit;
    TCHAR str[256] = { 0 };

    switch (iMessage)
    {
    case WM_CREATE:
        hWndMain = hWnd;
        rt.left = RegReadInt(SHCU, FullKeyPath(KEY, TEXT("Position")), TEXT("Left"), 0);
        rt.top = RegReadInt(SHCU, FullKeyPath(KEY, TEXT("Position")), TEXT("top"), 0);
        rt.right = RegReadInt(SHCU, FullKeyPath(KEY, TEXT("Position")), TEXT("right"), 300);
        rt.bottom = RegReadInt(SHCU, FullKeyPath(KEY, TEXT("Position")), TEXT("bottom"), 200);
        MoveWindow(hWnd, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, TRUE);
        hEdit = CreateWindow(TEXT("Edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 
            10, 10, 200, 25, hWnd, (HMENU)100, g_hInst, NULL);
        RegReadString(SHCU, FullKeyPath(KEY, TEXT("Edit")), TEXT("Str"), TEXT("¹®ÀÚ¿­"), str, 256);
        SetWindowText(hEdit, str);
        return 0;
    case WM_DESTROY:
        GetWindowRect(hWnd, &rt);
        auto check = RegWriteInt(SHCU, FullKeyPath(KEY, TEXT("Position")), TEXT("Left"), rt.left);
        if (!check)
            MessageBox(hWnd, 0, 0, MB_OK);
        RegWriteInt(SHCU, FullKeyPath(KEY, TEXT("Position")), TEXT("Top"), rt.top);
        RegWriteInt(SHCU, FullKeyPath(KEY, TEXT("Position")), TEXT("Right"), rt.right);
        RegWriteInt(SHCU, FullKeyPath(KEY, TEXT("Position")), TEXT("Bottom"), rt.bottom);
        GetWindowText(hEdit, str, 256);
        RegWriteString(SHCU, FullKeyPath(KEY, TEXT("Position")), TEXT("Str"), str);
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}