#include "pch.h"
#include "Video.h"
#include "Core.h"
#include <algorithm>
#include <string>
#include <format>

Video::Video()
{

}

Video::~Video()
{

}

bool Video::Load(fs::path fullPath)
{
	_parent = GET(Core)->GetHwnd();
	::GetClientRect(_parent, &_rc);

	// 1) 영상을 띄울 자식 창
	_videoWnd = ::CreateWindow(L"STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_BLACKRECT,
		0, 0, _rc.right, _rc.bottom, _parent, NULL, ::GetModuleHandle(NULL), NULL);

	_fullPath = fullPath;
	_alias = Utils::GetFileName(fullPath);

	// 2) 파일 열기 (alias = 이 영상을 부를 이름)
	wstring cmd1 = std::format(L"open \"{}\" type mpegvideo alias {}", fullPath.wstring(), _alias);
	MCIERROR err1 = ::mciSendString(cmd1.c_str(), NULL, 0, NULL);
	CheckMci(err1, L"open");

	// 3) 자식 창에 출력 + 크기 맞추기
	wstring cmd2 = std::format(L"window {} handle {}", _alias, (UINT_PTR)_videoWnd);
	MCIERROR err2 = ::mciSendString(cmd2.c_str(), NULL, 0, NULL);
	CheckMci(err2, L"window");

	WCHAR buf[64] = {};
	::mciSendString(std::format(L"where {} source", _alias).c_str(), buf, 64, NULL);   // "0 0 1920 1080"
	int32 x=0, y=0, vw=0, vh=0;
	swscanf_s(buf, L"%d %d %d %d", &x, &y, &vw, &vh);

	int32 left = 0, top = 0, w = _rc.right, h = _rc.bottom;
	if (vw > 0 && vh > 0)
	{
		float scale = min((float)_rc.right / vw, (float)_rc.bottom / vh);  // 넘치지 않는 쪽 기준
		w = (int32)(vw * scale);
		h = (int32)(vh * scale);
		left = (_rc.right - w) / 2;   // 가운데 정렬
		top = (_rc.bottom - h) / 2;
	}

	::mciSendString(std::format(L"put {} destination at {} {} {} {}", _alias, left, top, w, h).c_str(), NULL, 0, NULL);

	return false;
}

void Video::Play(bool loop)
{
	if (_isPlay)
		return;

	// 4) 재생
	CheckMci(::mciSendString(std::format(L"play {}", _alias).c_str(), NULL, 0, NULL), L"play");

	_isPlay = true;
}

void Video::Stop(bool reset)
{
	if (_isEnd)
		return;

	// 끝났거나 스킵할 때: 정리
	::mciSendString(std::format(L"close {}", _alias).c_str(), NULL, 0, NULL);
	::DestroyWindow(_videoWnd);

	_isEnd = true;
}

void Video::CheckEnd()
{
	// 매 프레임 (Update): 끝났는지 확인
	WCHAR mode[32] = {};
	// playing, stopped, paused, seeking
	::mciSendString(std::format(L"status {} mode", _alias).c_str(), mode, 32, NULL);
	bool checkEnd = (wcscmp(mode, L"stopped") == 0);
	if (checkEnd)
		Stop();
}


void Video::CheckMci(MCIERROR err, const WCHAR* what)
{
	if (err == 0) return;
	WCHAR msg[256] = {};
	::mciGetErrorString(err, msg, 256);
	::MessageBox(NULL, std::format(L"{}\n코드 {}: {}", what, err, msg).c_str(), L"MCI 에러", MB_OK);
}