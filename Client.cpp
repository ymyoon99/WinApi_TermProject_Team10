#include "stdafx.h"
#include "Client.h"
#include "GameFramework.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define CLIENT_WIDTH    800
#define CLIENT_HEIGHT   600

// Target max FPS (cap)
static constexpr double kTargetFps = 60.0;
static constexpr double kTargetFrameSec = 1.0 / kTargetFps;

HINSTANCE hInst;
LPCTSTR lpszClass = L"Winapi Term Project";
LPCTSTR lpszWindowName = L"Winapi Term Project";

static GameFramework gameframework;
static HWND g_hWnd = nullptr;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    timeBeginPeriod(1);

    // 고해상도 타이머로 계산
    MSG msg{};
    LARGE_INTEGER freq{};
    LARGE_INTEGER lastFrameStart{};
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&lastFrameStart);

    while (true)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else
        {
            // 프레임 시작 시간
            LARGE_INTEGER frameStart{};
            QueryPerformanceCounter(&frameStart);

            // 실제 Delta Time 계산
            double dtSec = double(frameStart.QuadPart - lastFrameStart.QuadPart) / double(freq.QuadPart);
            lastFrameStart = frameStart;

            // dt 폭주 방지
            if (dtSec > 0.1) dtSec = 0.1;

            // 화면 업데이트 및 갱신
            gameframework.Update(static_cast<float>(dtSec));
            InvalidateRect(g_hWnd, NULL, FALSE);

            // 60FPS 캡 설정
            LARGE_INTEGER afterWork{};
            QueryPerformanceCounter(&afterWork);

            double workSec = double(afterWork.QuadPart - frameStart.QuadPart) / double(freq.QuadPart);
            double remainSec = kTargetFrameSec - workSec;

            if (remainSec > 0.0)
            {
                DWORD sleepMs = static_cast<DWORD>(remainSec * 1000.0);
                if (sleepMs > 0) Sleep(sleepMs);
            }
        }
    }

    timeEndPeriod(1);

    gameframework.Clear();

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = lpszClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(
        lpszClass,
        lpszWindowName,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // 이 스타일로 윈도우 크기 조절 비활성화
        CW_USEDEFAULT,
        0,
        CLIENT_WIDTH,
        CLIENT_HEIGHT,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd)
    {
        return FALSE;
    }

    g_hWnd = hWnd;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 클라이언트 영역에 들어오면 커서를 숨기고, 나가면 보이도록 설정
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(tme);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = hWnd;
    TrackMouseEvent(&tme);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        gameframework.Create(hWnd);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        gameframework.Draw(hdc);
        EndPaint(hWnd, &ps);
    }
    break;

    //case WM_TIMER:
    //    break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            gameframework.TogglePause();
            break;
        }
    case WM_KEYUP:
        gameframework.OnKeyBoardProcessing(message, wParam, lParam);
        break;

    case WM_LBUTTONDOWN:
    case WM_MOUSEMOVE:
        gameframework.OnMouseProcessing(message, wParam, lParam);
        ShowCursor(FALSE); // 클라이언트 영역에서는 커서를 숨김
        break;

    case WM_MOUSELEAVE:
        ShowCursor(TRUE); // 클라이언트 영역을 벗어나면 커서를 보임
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
