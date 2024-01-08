#include <Windows.h>
#include <string>
#include <string_view>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("class");

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

/*정보 
* x, y, w, h : Rect 정보
* Mes : 애국가 가사 담긴 문자열
* interval : 시간 간격 */
struct SDataInfo
{
    int x, y, w, h;
    std::wstring str[4];
    int interval;
} Data[4] = {
    {10, 10, 180, 50, L"동해물과 백두산이", L"마르고 닳도록", L"하느님이 보우하사", L"우리나라만세", 250},
    {210, 10, 180, 50, L"남산위에 저 소나무", L"철갑을 두른듯", L"바람서리 불변함은", L"우리기상 일세", 500},
    {410, 10, 180, 50, L"가을하늘 공활한데", L"높고 구름없이", L"밝은달은 우리가슴", L"일편단심일세", 750},
    {610, 10, 180, 50, L"이 기상과 이 맘으로", L"충성을 다하여", L"괴로우나 즐거우나", L"나라사랑하세", 1000}
};

/*버튼 정보*/
const enum EButton : UINT
{
    ID_R1 = 0,  // 1절
    ID_R2,      // 2절
    ID_R3,      // 3절
    ID_R4,      // 4절
    ID_PAUSE,   // 정지
    ID_RUN      // 재개
};

/* 스레드 함수 */
DWORD WINAPI ThreadFunc(LPVOID Param)
{
    HDC hdc;
    SDataInfo* Data = (SDataInfo*)Param;
    int index = 0;
    hdc = GetDC(hWndMain);
    for (;;)
    {
        Rectangle(hdc, Data->x, Data->y, Data->x+Data->w, Data->y + Data->h);
        TextOut(hdc, Data->x + 5, Data->y + 5, Data->str[index % 4].c_str(), lstrlen(Data->str[index % 4].c_str()));
        Sleep(Data->interval);
        index++;
    }
    ReleaseDC(hWndMain, hdc);
    return 0;
}

/* CreateWindowW 래핑 함수*/
void CreateBotton(HWND& hWnd, std::wstring wstr, int x, DWORD mecro)
{
    CreateWindowW(L"button", wstr.c_str(), BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
        x, 120, 80, 25, hWnd, (HMENU)mecro, g_hInst, NULL);
}

/* CreateWindowW 래핑 함수*/
void CreateBanner(HWND& hWnd, std::wstring wstr, int x, DWORD mecro)
{
    CreateWindowW(L"button", wstr.c_str(), WS_CHILD | WS_VISIBLE |BS_AUTORADIOBUTTON,
        x, 60, 100, 35, hWnd, (HMENU)mecro, g_hInst, NULL);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    static HANDLE hThread[4];
    static DWORD ThreadID[4];
    static int NowThread = 0;
    int i;

    switch (iMessage)
    {
    case WM_CREATE:
        hWndMain = hWnd;
        CreateBotton(hWnd, L"실행", 310, ID_RUN);
        CreateBotton(hWnd, L"중지", 410, ID_PAUSE);
        CreateBanner(hWnd, L"1절 스레드",  60, ID_R1);
        CreateBanner(hWnd, L"2절 스레드", 260, ID_R2);
        CreateBanner(hWnd, L"3절 스레드", 460, ID_R3);
        CreateBanner(hWnd, L"4절 스레드", 660, ID_R4);
        CheckRadioButton(hWnd, ID_R1, ID_R4, ID_R1);

        for (i = 0; i < 4; ++i)
            hThread[i] = CreateThread(NULL, 0, ThreadFunc, &Data[i], 0, &ThreadID[i]);
        
        return 0;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_R1:
            NowThread = 0;
            break;
        case ID_R2:
            NowThread = 1;
            break;
        case ID_R3:
            NowThread = 2;
            break;
        case ID_R4:
            NowThread = 3;
            break;
        case ID_PAUSE:
            SuspendThread(hThread[NowThread]);
            break;
        case ID_RUN:
            ResumeThread(hThread[NowThread]);
            break;
        }
        return 0;
    case WM_DESTROY:
        for (i = 0; i < 4; ++i)
            CloseHandle(hThread[i]);
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}