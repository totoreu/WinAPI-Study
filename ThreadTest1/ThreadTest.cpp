#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("ThreadTest");

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

/* Rect별 색상*/
enum ECOLOR
{
    RED = 0,
    GREEN,
    BLUE,
    DEFAULT // WHITE
};

/* RECT 데이터가 담긴 구조체*/
typedef struct SRectData
{
    RECT s_rc = { 0,0,0,0 };
    ECOLOR s_color = DEFAULT;
    BYTE s_red = 0;
    BYTE s_green = 0;
    BYTE s_blue = 0;
} RECT_DATA, * PRECT_DATA;

/* 스레드 시작 함수*/
DWORD WINAPI ThreadFunc(LPVOID prd)
{
    HDC hdc;
    BYTE Blue = 0;
    HBRUSH hBrush, hOldBrush;
    RECT_DATA rd = *(PRECT_DATA)prd;
    hdc = GetDC(hWndMain);
    for (;;)
    {
        Blue += 5;
        Sleep(20);
        hBrush = CreateSolidBrush(RGB(rd.s_red, rd.s_green, rd.s_blue));

        switch (rd.s_color)
        {
        case RED:
            rd.s_red += 5;
            break;
        case GREEN:
            rd.s_green += 5;
            break;
        case BLUE:
            rd.s_blue += 5;
            break;
        case DEFAULT:
            rd.s_red += 5;
            rd.s_green += 5;
            rd.s_blue += 5;
            break;
        }

        hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
        Rectangle(hdc, rd.s_rc.left, rd.s_rc.top, rd.s_rc.right, rd.s_rc.bottom);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hBrush);
    }
    ReleaseDC(hWndMain, hdc);
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    DWORD ThreadID;
    HANDLE hThread;

    static RECT_DATA arrData[] = {
        {{10, 10, 100, 100}, RED}, 
        {{110, 10, 200, 100}, GREEN},
        {{10, 110, 100, 200}, BLUE},
        {{110, 110, 200, 200}},
    };

    switch (iMessage)
    {
    case WM_CREATE:
        hWndMain = hWnd;
        for(int i = 0; i<4; i++)
        {
            CloseHandle(CreateThread(NULL, 0, ThreadFunc, &arrData[i], 0, &ThreadID));
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}