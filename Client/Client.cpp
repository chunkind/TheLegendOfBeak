#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "Core.h"

HINSTANCE hInst;
HWND g_hWnd;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void OnDebugMode(bool isOn)
{
    if (isOn) {
        // 디버그용 콘솔 창 띄우기
        AllocConsole();
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
    }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    OnDebugMode(false); // 디버그용 콘솔

    // 1) 윈도우 창 정보 등록
    MyRegisterClass(hInstance);

    // 2) 윈도우 창 생성
    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    GET(Core)->Init(g_hWnd);

    MSG msg = {};
    uint64 prevTick = 0;

    // 3) 메인 루프
    // - 입력
    // - 로직
    // - 렌더링
    while (WM_QUIT != msg.message)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else
        {
            uint64 now = ::GetTickCount64();

            if (now - prevTick >= 10) {
                GET(Core)->Update();
                GET(Core)->Render();

                prevTick = now;
            }
        }
    }

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
    wcex.hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_Client));
    wcex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"key";
    wcex.hIconSm = ::LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return ::RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    RECT windowRect = { 0, 0, GWinSizeX, GWinSizeY };
    ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

    g_hWnd = ::CreateWindowW(L"key", GAME_TITLE.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, hInstance, nullptr);

    if (!g_hWnd)
    {
        return FALSE;
    }

    ::ShowWindow(g_hWnd, nCmdShow);
    ::UpdateWindow(g_hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_EXIT:
            ::DestroyWindow(hWnd);
            break;
        default:
            return ::DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = ::BeginPaint(hWnd, &ps);
        ::EndPaint(hWnd, &ps);
    }
    break;
    case WM_SIZING:
    {
        // 창 크기 조절 시 4:3 비율 유지 + 최대 크기 제한
        RECT* r = (RECT*)lParam;
        int32 w = r->right - r->left;
        int32 h = r->bottom - r->top;

        switch (wParam)
        {
        // 가로 또는 대각선 방향 드래그 -> 가로 기준으로 세로 맞춤
        case WMSZ_LEFT: case WMSZ_RIGHT:
        case WMSZ_TOPLEFT: case WMSZ_BOTTOMLEFT:
        case WMSZ_TOPRIGHT: case WMSZ_BOTTOMRIGHT:
            if (w > MAX_WIN_WIDTH) w = MAX_WIN_WIDTH;
            h = w * 3 / 4;
            r->right = r->left + w;
            r->bottom = r->top + h;
            break;
        // 세로 방향 드래그 -> 세로 기준으로 가로 맞춤
        case WMSZ_TOP: case WMSZ_BOTTOM:
            if (h > MAX_WIN_HEIGHT) h = MAX_WIN_HEIGHT;
            w = h * 4 / 3;
            r->right = r->left + w;
            r->bottom = r->top + h;
            break;
        }
        return TRUE;
    }
    case WM_GETMINMAXINFO:
    {
        // 최대화 버튼 클릭 시 최대 크기 제한 + 화면 가운데 정렬
        MINMAXINFO* mmi = (MINMAXINFO*)lParam;
        int32 screenW = ::GetSystemMetrics(SM_CXSCREEN);
        int32 screenH = ::GetSystemMetrics(SM_CYSCREEN);

        // 최대화 시 창 크기
        mmi->ptMaxSize.x = MAX_WIN_WIDTH;
        mmi->ptMaxSize.y = MAX_WIN_HEIGHT;
        // 최대화 시 창 위치 (모니터 가운데)
        mmi->ptMaxPosition.x = (screenW - MAX_WIN_WIDTH) / 2;
        mmi->ptMaxPosition.y = (screenH - MAX_WIN_HEIGHT) / 2;
        return 0;
    }
    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;
    default:
        return ::DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
