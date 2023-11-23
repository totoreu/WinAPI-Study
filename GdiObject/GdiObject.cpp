#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("GdiObject");

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
    WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);

    hWnd = CreateWindow(
        lpszClass,
        lpszClass,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        400, 300,
        NULL,
        (HMENU)NULL,
        hInstance,
        NULL
    );

    ShowWindow(hWnd, nCmdShow);

    HACCEL hAccel;
    hAccel = LoadAccelerators(g_hInst, MAKEINTRESOURCE(IDR_ACCELERATOR2));
    while (GetMessage(&Message, NULL, 0, 0))
    {
        if (!TranslateAccelerator(hWnd, hAccel, &Message))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
    }
    return (int)Message.wParam;
}

enum ESelect : INT8
{
    ESELECT_PEN = 0,
    ESELECT_LINE,
    ESELECT_RECT,
    ESELECT_ELLIPSE
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    static bool bClick = false;
    static int sx = 0, sy = 0 , ex = 0, ey = 0;
    static ESelect Select;

    switch (iMessage)
    {
    case WM_CREATE:
        hWndMain = hWnd;
        Select = ESELECT_PEN;
        return 0;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_LINE_PEN:
            Select = ESELECT_PEN;
            break;
        case ID_LINE_LINE:
            Select = ESELECT_LINE;
            break;
        case ID_RECT:
            Select = ESELECT_RECT;
            break;
        case ID_ELLIPSE:
            Select = ESELECT_ELLIPSE;
            break;
        }
        return 0;
    case WM_LBUTTONDOWN:
        sx = LOWORD(lParam);
        sy = HIWORD(lParam);
        ex = sx;
        ey = sy;
        bClick = true;
        return 0;
    case WM_LBUTTONUP:
        bClick = false;
        hdc = GetDC(hWnd);
        ReleaseDC(hWnd, hdc);
        return 0;
    case WM_MOUSEMOVE:
        if (bClick)
        {
            hdc = GetDC(hWnd);
            switch (Select)
            {
            case ESELECT_PEN:
                MoveToEx(hdc, sx, sy, NULL);
                sx = LOWORD(lParam);
                sy = HIWORD(lParam);
                LineTo(hdc, sx, sy);
                break;
            case ESELECT_LINE:
                SetROP2(hdc, R2_NOT);
                MoveToEx(hdc, sx, sy, NULL);
                LineTo(hdc, ex, ey);
                ex = LOWORD(lParam);
                ey = HIWORD(lParam);
                MoveToEx(hdc, sx, sy, NULL);
                LineTo(hdc, ex, ey);
                break;
            case ESELECT_RECT:
                ex = LOWORD(lParam);
                ey = HIWORD(lParam);
                Rectangle(hdc, sx, sy, ex, ey);
                SetROP2(hdc, R2_NOT);
                break;
            case ESELECT_ELLIPSE:
                ex = LOWORD(lParam);
                ey = HIWORD(lParam);
                Ellipse(hdc, sx, sy, ex, ey);
                SetROP2(hdc, R2_NOT);
                break;
            }
            ReleaseDC(hWnd, hdc);
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}