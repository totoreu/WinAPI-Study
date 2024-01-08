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

/*���� 
* x, y, w, h : Rect ����
* Mes : �ֱ��� ���� ��� ���ڿ�
* interval : �ð� ���� */
struct SDataInfo
{
    int x, y, w, h;
    std::wstring str[4];
    int interval;
} Data[4] = {
    {10, 10, 180, 50, L"���ع��� ��λ���", L"������ �⵵��", L"�ϴ����� �����ϻ�", L"�츮���󸸼�", 250},
    {210, 10, 180, 50, L"�������� �� �ҳ���", L"ö���� �θ���", L"�ٶ����� �Һ�����", L"�츮��� �ϼ�", 500},
    {410, 10, 180, 50, L"�����ϴ� ��Ȱ�ѵ�", L"���� ��������", L"�������� �츮����", L"����ܽ��ϼ�", 750},
    {610, 10, 180, 50, L"�� ���� �� ������", L"�漺�� ���Ͽ�", L"���ο쳪 ��ſ쳪", L"�������ϼ�", 1000}
};

/*��ư ����*/
const enum EButton : UINT
{
    ID_R1 = 0,  // 1��
    ID_R2,      // 2��
    ID_R3,      // 3��
    ID_R4,      // 4��
    ID_PAUSE,   // ����
    ID_RUN      // �簳
};

/* ������ �Լ� */
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

/* CreateWindowW ���� �Լ�*/
void CreateBotton(HWND& hWnd, std::wstring wstr, int x, DWORD mecro)
{
    CreateWindowW(L"button", wstr.c_str(), BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
        x, 120, 80, 25, hWnd, (HMENU)mecro, g_hInst, NULL);
}

/* CreateWindowW ���� �Լ�*/
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
        CreateBotton(hWnd, L"����", 310, ID_RUN);
        CreateBotton(hWnd, L"����", 410, ID_PAUSE);
        CreateBanner(hWnd, L"1�� ������",  60, ID_R1);
        CreateBanner(hWnd, L"2�� ������", 260, ID_R2);
        CreateBanner(hWnd, L"3�� ������", 460, ID_R3);
        CreateBanner(hWnd, L"4�� ������", 660, ID_R4);
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