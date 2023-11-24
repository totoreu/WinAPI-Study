#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("FileWrite");

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
    HDC hdc;
    PAINTSTRUCT ps;
    HANDLE hFile;
    static DWORD dwWrite, dwError;
    static RECT rt;
    static TCHAR str[256] = TEXT("안녕하세요 파일을 저장해보겠습니다.");

    switch (iMessage)
    {
    case WM_CREATE:
        hWndMain = hWnd;
        return 0;
    case WM_LBUTTONDOWN:
        hFile = CreateFile(TEXT("saveFile.txt"), GENERIC_WRITE, 0, NULL,
            CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            WORD bom = 0xFEFF;
            WriteFile(hFile, &bom, 2, 0, NULL);
            if (WriteFile(hFile, str, 256, &dwWrite, NULL))
            {
                CloseHandle(hFile);
                InvalidateRect(hWnd, &rt, TRUE);
                TCHAR Meesage[32] = TEXT("성공");
                MessageBox(hWnd, Meesage, TEXT("알림"), MB_OK);
            }
            else
            {
                dwError = GetLastError();
                TCHAR errorMessage[256] = { 0 };
                wsprintf(errorMessage, TEXT("FileWrite failed with error %d"), dwError);
                MessageBox(hWnd, errorMessage, TEXT("Error"), MB_OK | MB_ICONERROR);
            }
        }
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &rt);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}