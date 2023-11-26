#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("FileIni");

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    RECT rt;
    TCHAR buf[10];
    TCHAR fileName[32] = TEXT("IniFile.ini");

    switch (iMessage)
    {
    case WM_CREATE:
        hWndMain = hWnd;
        rt.left = GetPrivateProfileInt(TEXT("Position"), TEXT("Left"), 0, fileName);
        rt.top = GetPrivateProfileInt(TEXT("Position"), TEXT("Top"), 0, fileName);
        rt.right = GetPrivateProfileInt(TEXT("Position"), TEXT("Right"), 100, fileName);
        rt.bottom = GetPrivateProfileInt(TEXT("Position"), TEXT("Bottom"), 100, fileName);
        MoveWindow(hWnd, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, TRUE);
        return 0;
    case WM_DESTROY:
        GetWindowRect(hWnd, &rt);
        wsprintf(buf, TEXT("%d"), rt.left);
        auto Test = WritePrivateProfileString(TEXT("Position"), TEXT("Left"), buf, fileName);
        if (!Test)
        {
            DWORD error = GetLastError();
            TCHAR errorCode[10];
            wsprintf(errorCode, TEXT("%d"), error);
            MessageBox(NULL, errorCode, TEXT("Error"), MB_OK | MB_ICONERROR);
        }
        wsprintf(buf, TEXT("%d"), rt.top);
        WritePrivateProfileString(TEXT("Position"), TEXT("Top"), buf, fileName);
        wsprintf(buf, TEXT("%d"), rt.right);
        WritePrivateProfileString(TEXT("Position"), TEXT("Right"), buf, fileName);
        wsprintf(buf, TEXT("%d"), rt.bottom);
        WritePrivateProfileString(TEXT("Position"), TEXT("Bottom"), buf, fileName);
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}