#include "pch.h"
#include "Core.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResMgr.h"
#include "SoundMgr.h"
#include "NetMgr.h"

Core::Core()
{

}

Core::~Core()
{
	// 메모리 누수 체크
	//GET(SceneMgr)->Clear();
	//GET(ResMgr)->Clear();

	_CrtDumpMemoryLeaks();
}

void Core::Init(HWND hwnd)
{
	_hwnd = hwnd;
	hdc = ::GetDC(hwnd);

	::GetClientRect(hwnd, &_rect);

	hdcBack = ::CreateCompatibleDC(hdc); // hdc와 호환되는 DC를 생성
	_bmpBack = ::CreateCompatibleBitmap(hdc, _rect.right, _rect.bottom); // hdc와 호환되는 비트맵 생성
	HBITMAP prev = (HBITMAP)::SelectObject(hdcBack, _bmpBack); // DC에 BMP를 연결
	::DeleteObject(prev);

	GET(TimeMgr)->Init();
	GET(InputMgr)->Init(hwnd);
	GET(SceneMgr)->Init();
	GET(ResMgr)->Init(hwnd, fs::path(L"C:\\git\\"+ GAME_TITLE + L"\\Resources"));

	GET(SoundMgr)->Init(hwnd);

	GET(SceneMgr)->ChangeScene(SceneType::MenuScene);

	GET(NetMgr)->Init();
}

void Core::Update()
{
	GET(TimeMgr)->Update();
	GET(InputMgr)->Update();
	GET(SceneMgr)->Update();
	GET(NetMgr)->Update();
}

void Core::Render()
{
	GET(SceneMgr)->Render(hdcBack);

	uint32 fps = GET(TimeMgr)->GetFps();

	{
		POINT mousePos = GET(InputMgr)->GetMousePos();
		wstring str = std::format(L"Mouse({0}, {1})", mousePos.x, mousePos.y);
		::TextOut(hdcBack, 20, 10, str.c_str(), static_cast<int32>(str.size()));
	}

	{
		wstring str = std::format(L"FPS({0}), DT({1})", fps, DT);
		::TextOut(hdcBack, 550, 10, str.c_str(), static_cast<int32>(str.size()));
	}

	// 더블 버퍼링: 백버퍼(800x600)를 실제 윈도우 크기에 맞춰 확대/축소하여 표시
	RECT clientRect;
	::GetClientRect(_hwnd, &clientRect); // 현재 윈도우 클라이언트 영역 크기
	::SetStretchBltMode(hdc, HALFTONE); // 확대 시 화질 보정 (픽셀 깨짐 방지)
	::StretchBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcBack, 0, 0, _rect.right, _rect.bottom, SRCCOPY);
	::PatBlt(hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS); // 백버퍼 초기화 (흰색)
}
